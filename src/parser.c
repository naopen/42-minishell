/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:17 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/08 17:08:25 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (cmd == NULL)
		fatal_error("malloc");
	cmd->argv = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_redirect	*new_redirect(t_token *tokens)
{
	t_redirect	*redir;

	redir = (t_redirect *)malloc(sizeof(t_redirect));
	if (redir == NULL)
		fatal_error("malloc");
	if (tokens->str[0] == '>')
	{
		if (tokens->str[1] == '>')
			redir->type = REDIR_APPEND;
		else
			redir->type = REDIR_OUTPUT;
		redir->fd = 1;
	}
	else if (tokens->str[0] == '<')
	{
		if (tokens->str[1] == '<')
			redir->type = REDIR_HEREDOC;
		else
			redir->type = REDIR_INPUT;
		redir->fd = 0;
	}
	tokens = tokens->next;
	redir->filename = ft_strdup(tokens->str);
	if (redir->filename == NULL)
		fatal_error("malloc");
	redir->next = NULL;
	return (redir);
}

void	append_tok(t_token **tokens, t_token *tok)
{
	t_token	*tmp;

	if (*tokens == NULL)
	{
		*tokens = tok;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = tok;
}

void	append_redirect(t_redirect **redirects, t_redirect *redir)
{
	t_redirect	*tmp;

	if (*redirects == NULL)
	{
		*redirects = redir;
		return ;
	}
	tmp = *redirects;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redir;
}

t_command	*parse(t_token *tokens)
{
	t_command	*head;
	t_command	*cmd;
	t_token		*tok;
	int			argc;

	head = NULL;
	cmd = NULL;
	while (tokens->kind != TK_EOF)
	{
		if (head == NULL)
		{
			head = new_command();
			cmd = head;
		}
		else
		{
			cmd->next = new_command();
			cmd = cmd->next;
		}
		argc = 0;
		tok = tokens;
		while (tok->kind != TK_EOF && tok->kind != TK_PIPE)
		{
			if (tok->kind == TK_WORD)
				argc++;
			tok = tok->next;
		}
		cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
		if (cmd->argv == NULL)
			fatal_error("malloc");
		argc = 0;
		while (tokens->kind != TK_EOF && tokens->kind != TK_PIPE)
		{
			if (tokens->kind == TK_WORD)
				cmd->argv[argc++] = ft_strdup(tokens->str);
			else if (tokens->kind == TK_REDIR_OUTPUT || tokens->kind == TK_REDIR_APPEND
					|| tokens->kind == TK_REDIR_INPUT || tokens->kind == TK_REDIR_HEREDOC)
				append_redirect(&cmd->redirects, new_redirect(tokens));
			tokens = tokens->next;
		}
		cmd->argv[argc] = NULL;
		if (tokens->kind == TK_PIPE)
			tokens = tokens->next;
	}
	return (head);
}
