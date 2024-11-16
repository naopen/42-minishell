/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:17 by nkannan           #+#    #+#             */
/*   Updated: 2024/11/16 16:00:16 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node	*new_node(t_node_type type)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (node == NULL)
		exit_with_error("minishell: malloc error");
	node->type = type;
	node->argv = NULL;
	node->redirects = NULL;
	node->next = NULL;
	return (node);
}

static char	**get_command_args(t_token **token_list)
{
	int		argc;
	char	**argv;
	t_token	*token;

	argc = 0;
	token = *token_list;
	while (token && token->type == TOKEN_WORD)
	{
		argc++;
		token = token->next;
	}
	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (argv == NULL)
		exit_with_error("minishell: malloc error");
	argc = 0;
	token = *token_list;
	while (token && token->type == TOKEN_WORD)
	{
		argv[argc++] = token->word;
		token = token->next;
	}
	argv[argc] = NULL;
	*token_list = token;
	return (argv);
}

static t_redirect	*parse_redirect(t_token **token_list)
{
	t_redirect	*redirect;

	redirect = (t_redirect *)malloc(sizeof(t_redirect));
	if (redirect == NULL)
		exit_with_error("minishell: malloc error");
	if ((*token_list)->type == TOKEN_REDIRECT_IN)
		redirect->type = REDIRECT_IN;
	else if ((*token_list)->type == TOKEN_REDIRECT_OUT)
		redirect->type = REDIRECT_OUT;
	else if ((*token_list)->type == TOKEN_REDIRECT_APPEND)
		redirect->type = REDIRECT_APPEND;
	else if ((*token_list)->type == TOKEN_HEREDOC)
	{
		redirect->type = REDIRECT_HEREDOC;
		*token_list = (*token_list)->next;
		if (*token_list == NULL || (*token_list)->type != TOKEN_WORD)
			exit_with_error("minishell: syntax error: heredoc delimiter not found");
		redirect->file_name = (*token_list)->word;
	}
	else
		exit_with_error("minishell: syntax error: invalid redirect");
	*token_list = (*token_list)->next;
	return (redirect);
}

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
}

static t_node	*parse_command(t_token **token_list)
{
	t_node		*command;
	t_redirect	*redirect_head;
	t_redirect	*redirect;

	command = new_node(NODE_COMMAND);
	command->argv = get_command_args(token_list);
	redirect_head = NULL;
	while (*token_list && ((*token_list)->type == TOKEN_REDIRECT_IN
			|| (*token_list)->type == TOKEN_REDIRECT_OUT
			|| (*token_list)->type == TOKEN_REDIRECT_APPEND
			|| (*token_list)->type == TOKEN_HEREDOC))
	{
		redirect = parse_redirect(token_list);
		add_redirect_to_list(&redirect_head, redirect);
	}
	command->redirects = redirect_head;
	return (command);
}

static t_node	*parse_pipeline(t_token **token_list)
{
	t_node	*head;
	t_node	*node;

	head = parse_command(token_list);
	node = head;
	while (*token_list && (*token_list)->type == TOKEN_OPERATOR
		&& ft_strcmp((*token_list)->word, "|") == 0)
	{
		*token_list = (*token_list)->next;
		node->next = parse_command(token_list);
		node = node->next;
	}
	return (head);
}

t_node	*parse(t_token **token_list)
{
	return (parse_pipeline(token_list));
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
