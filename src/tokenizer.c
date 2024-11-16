/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 17:09:19 by nkannan           #+#    #+#             */
/*   Updated: 2024/11/16 16:00:20 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_token_type	get_token_type(const char *str)
{
	if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_REDIRECT_APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (strchr(";|", *str))
		return (TOKEN_OPERATOR);
	return (TOKEN_WORD);
}

static t_token	*new_token(t_token_type type, char *word)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (token == NULL)
		exit_with_error("minishell: malloc error");
	token->type = type;
	token->word = word;
	token->next = NULL;
	return (token);
}

void	free_token_list(t_token *token_list)
{
	t_token	*tmp;

	while (token_list)
	{
		tmp = token_list->next;
		free(token_list->word);
		free(token_list);
		token_list = tmp;
	}
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

static t_token	*split_token(char **line)
{
	t_token	*token;
	char	*word;
	char	*start;
	char	quote;

	start = *line;
	while (**line && !ft_strchr(" \t<>|;&", **line))
	{
		if (is_quote(**line))
		{
			quote = **line;
			(*line)++;
			while (**line && **line != quote)
				(*line)++;
			if (**line == quote)
				(*line)++;
			else
				exit_with_error("minishell: syntax error: unclosed quote");
		}
		else
			(*line)++;
	}
	word = ft_strndup(start, *line - start);
	if (word == NULL)
		exit_with_error("minishell: malloc error");
	token = new_token(get_token_type(word), word);
	if (token == NULL)
	{
		free(word);
		exit_with_error("minishell: malloc error");
	}
	return (token);
}

static void	add_token_to_list(t_token **head, t_token *token)
{
	t_token	*tmp;

	if (*head == NULL)
		*head = token;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = token;
	}
}

t_token	*tokenize(char *line)
{
	t_token	*head;
	t_token	*token;

	head = NULL;
	while (*line)
	{
		while (*line == ' ' || *line == '\t')
			line++;
		token = split_token(&line);
		add_token_to_list(&head, token);
	}
	token = new_token(TOKEN_EOF, NULL);
	add_token_to_list(&head, token);
	return (head);
}
