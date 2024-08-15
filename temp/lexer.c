/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:07:38 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/15 17:36:00 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*new_token(t_token_kind kind, char *str)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (tok == NULL)
		exit_with_error("malloc");
	tok->kind = kind;
	tok->str = str;
	tok->next = NULL;
	return (tok);
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

t_token	*tokenize(char *line)
{
	t_token	head;
	t_token	*tok;
	char	*start;
	bool	escaped;

	head.next = NULL;
	tok = &head;
	while (*line)
	{
		escaped = false;
		while (*line == ' ')
			line++;
		if (*line == '\0')
			break ;
		start = line;
		if (is_quote(*line))
		{
			char	quote;

			quote = *line++;
			while (*line && (escaped || *line != quote))
			{
				if (*line == '\\' && !escaped && quote == '"')
					escaped = true;
				else
					escaped = false;
				line++;
			}
			if (*line == quote)
				line++;
			if (quote == '\'')
				tok->next = new_token(TK_WORD,
						ft_strndup(start + 1, line - start - 2));
			else
				tok->next = new_token(TK_WORD, ft_substr(start, 1, line - start - 2));
		}
		else if (*line == '|')
		{
			tok->next = new_token(TK_PIPE, ft_strdup("|"));
			line++;
		}
		else if (*line == '>' || *line == '<')
		{
			if (line[1] == *line)
			{
				tok->next = new_token(TK_REDIR_OUTPUT, ft_strndup(line, 2));
				line += 2;
			}
			else
			{
				tok->next = new_token(TK_REDIR_OUTPUT, ft_strndup(line, 1));
				line++;
			}
		}
		else
		{
			while (*line != ' ' && *line != '\0' && !is_quote(*line)
				&& *line != '|' && *line != '>' && *line != '<')
			{
				if (*line == '\\' && *(line + 1) == '\n')
					line++;
				line++;
			}
			tok->next = new_token(TK_WORD, ft_strndup(start, line - start));
		}
		if (tok->next == NULL)
			exit_with_error("malloc");
		tok = tok->next;
	}
	tok->next = new_token(TK_EOF, NULL);
	return (head.next);
}
