/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:17 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/02 12:00:13 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node	*new_node(t_node_type type)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (node == NULL)
		exit_with_error();
	node->type = type;
	node->argv = NULL;
	node->redirects = NULL;
	node->next = NULL;
	return (node);
}

static char	**get_command_args(t_token **token_list, int argc)
{
	char	**strs;
	t_token	*token;

	token = *token_list;
	strs = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!strs)
		exit_with_error();
	argc = 0;
	while (token->type != TOKEN_OPERATOR && token->type != TOKEN_EOF)
	{
		if (token->type != TOKEN_WORD)
			token = token->next->next;
		else
		{
			strs[argc++] = expand_env_var(ft_strdup(token->word));
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
// 		exit_with_error();
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
// 			custum_error("minishell: syntax error: heredoc delimiter not found", 1);
// 		redirect->file_name = (*token_list)->word;
// 	}
// 	else
// 		custum_error("minishell: syntax error: invalid redirect", 1);
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

char	*get_filename(t_token *token)
{
	char	*filename;

	filename = NULL;
	if (token->type == TOKEN_EOF || is_metachar(token->word[0]))
		syntax_error(token);
	else
		filename = ft_strdup(token->word);
	return (filename);
}

void	add_redirect(t_node *node, t_token **token_list)
{
	t_token		*token;
	t_redirect	*red;

	token = *token_list;
	red = (t_redirect *)malloc(sizeof(t_redirect) * 1);
	if (!red)
		exit_with_error();
	red->next = NULL;
	red->type = (t_redirect_type)token->type;
	token = token->next;
	red->file_name = get_filename(token);
	add_redirect_to_list(&(node->redirects), red);
	*token_list = token;
	return ;
}

static t_node	*parse_process(t_token **token_list)
{
	t_node		*command;
	t_token		*token;
	int			argc;

	token = *token_list;
	argc = 0;
	command = new_node(NODE_COMMAND);
	while (token->type != TOKEN_OPERATOR && token->type != TOKEN_EOF)
	{
		if (token->type != TOKEN_WORD)
			add_redirect(command, &token);
		else
		{
			argc++;
			token = token->next;
		}
	}
	command->argv = get_command_args(token_list, argc);
	return (command);
}

t_node	*parse(t_token **token_list)
{
	t_node	*head;
	t_node	*node;

	head = parse_process(token_list);
	node = head;
	while (*token_list && (*token_list)->type == TOKEN_OPERATOR
		&& ft_strcmp((*token_list)->word, "|") == 0)
	{
		*token_list = (*token_list)->next;
		node->next = parse_process(token_list);
		node = node->next;
	}
	return (head);
}

void	free_ast(t_node *ast)
{
	t_node		*tmp_node;
	t_redirect	*tmp_redirect;

	while (ast)
	{
		tmp_node = ast->next;
		ft_strarrdel(ast->argv);
		while (ast->redirects)
		{
			tmp_redirect = ast->redirects->next;
			free(ast->redirects->file_name);
			free(ast->redirects);
			ast->redirects = tmp_redirect;
		}
		free(ast);
		ast = tmp_node;
	}
}
