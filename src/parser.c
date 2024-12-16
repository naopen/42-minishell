/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:17 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 16:27:42 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node	*new_node(t_mini *mini, t_node_type type)
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

char	*get_filename(t_mini *mini, t_token *token)
{
	char	*filename;

	filename = NULL;
	if (token->type == TOKEN_EOF || is_metachar(token->word[0]))
		syntax_error(mini, token);
	else
	{
		token->word = cut_quote(mini, token->word, 0, 0);
		filename = expand_env_var(mini, ft_strdup(token->word));
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

t_node	*parse_process(t_mini *mini, t_token **token_list)
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

	if (mini->unable)
		return (NULL);
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
