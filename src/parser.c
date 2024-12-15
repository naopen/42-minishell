/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:17 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/15 19:09:41 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node	*new_node(t_mini *mini, t_node_type type)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (node == NULL)
		system_error(mini);
	node->type = type;
	node->argv = NULL;
	node->redirects = NULL;
	node->next = NULL;
	return (node);
}

static char	**get_command_args(t_mini *mini, t_token **token_list, int argc)
{
	char	**strs;
	t_token	*token;

	token = *token_list;
	strs = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!strs)
		system_error(mini);
	argc = 0;
	while (token->type != TOKEN_OPERATOR && token->type != TOKEN_EOF)
	{
		if (token->type != TOKEN_WORD)
			token = token->next->next;
		else
		{
			strs[argc++] = expand_env_var(mini, ft_strdup(token->word));
			token = token->next;
		}
	}
	strs[argc] = NULL;
	*token_list = token;
	return (strs);
}

// static t_redirect	*parse_redirect(t_token **token_list)
// {
// 	t_redirect	*redirect;

// 	redirect = (t_redirect *)malloc(sizeof(t_redirect));
// 	if (redirect == NULL)
// 		system_error(mini);
// 	if ((*token_list)->type == TOKEN_REDIRECT_IN)
// 		redirect->type = REDIRECT_IN;
// 	else if ((*token_list)->type == TOKEN_REDIRECT_OUT)
// 		redirect->type = REDIRECT_OUT;
// 	else if ((*token_list)->type == TOKEN_REDIRECT_APPEND)
// 		redirect->type = REDIRECT_APPEND;
// 	else if ((*token_list)->type == TOKEN_HEREDOC)
// 	{
// 		redirect->type = REDIRECT_HEREDOC;
// 		*token_list = (*token_list)->next;
// 		if (*token_list == NULL || (*token_list)->type != TOKEN_WORD)
// 			custom_error("minishell: syntax error: heredoc delimiter not found", 1);
// 		redirect->file_name = (*token_list)->word;
// 	}
// 	else
// 		custom_error("minishell: syntax error: invalid redirect", 1);
// 	*token_list = (*token_list)->next;
// 	return (redirect);
// }

static void	add_redirect_to_list(t_redirect **head, t_redirect *redirect)
{
	t_redirect	*tmp;

	if (*head == NULL)
		*head = redirect;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redirect;
	}
	return ;
}

char *cut_quote(t_mini *mini, char *word, int single_q, int double_q)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = (char *)malloc(sizeof(char) * (ft_strlen(word) + 1));
	if (!new)
	{
		free(word);
		system_error(mini);
	}
	while (word[i] != '\0')
	{
		if (word[i] == '\'' && !double_q)
			single_q = !single_q;
		else if (word[i] == '\"' && !single_q)
			double_q = !double_q;
		else
			new[j++] = word[i];
		i++;
	}
	new[j] = '\0';
	free(word);
	return (new);
}

char	*get_filename(t_mini *mini, t_token *token)
{
	char	*filename;

	filename = NULL;
	if (token->type == TOKEN_EOF || is_metachar(token->word[0]))
		syntax_error(mini, token);
	else
	{
		token->word = cut_quote(mini, token->word, 0, 0);
		filename = ft_strdup(token->word);
	}
	return (filename);
}

void	add_redirect(t_mini *mini, t_node *node, t_token **token_list)
{
	t_token		*token;
	t_redirect	*red;

	token = *token_list;
	red = (t_redirect *)malloc(sizeof(t_redirect) * 1);
	if (!red)
		system_error(mini);
	red->next = NULL;
	red->type = (t_redirect_type)token->type;
	token = token->next;
	red->file_name = get_filename(mini, token);
	add_redirect_to_list(&(node->redirects), red);
	*token_list = token;
	return ;
}

static t_node	*parse_process(t_mini *mini, t_token **token_list)
{
	t_node		*command;
	t_token		*token;
	int			argc;

	token = *token_list;
	argc = 0;
	command = new_node(mini, NODE_COMMAND);
	while (token->type != TOKEN_OPERATOR && token->type != TOKEN_EOF)
	{
		if (token->type != TOKEN_WORD)
			add_redirect(mini, command, &token);
		else
		{
			argc++;
			token = token->next;
		}
	}
	command->argv = get_command_args(mini, token_list, argc);
	return (command);
}

t_node	*parse(t_mini *mini, t_token **token_list)
{
	t_node	*head;
	t_node	*node;
	t_token	*token_head;

	token_head = *token_list;
	head = parse_process(mini, token_list);
	node = head;
	while (*token_list && (*token_list)->type == TOKEN_OPERATOR
		&& ft_strcmp((*token_list)->word, "|") == 0)
	{
		*token_list = (*token_list)->next;
		node->next = parse_process(mini, token_list);
		node = node->next;
	}
	*token_list = token_head;
	return (head);
}
