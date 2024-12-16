/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 17:09:19 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 13:12:51 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token_type	get_token_type(const char *str)
{
	if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_REDIRECT_APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strchr(";|", *str))
		return (TOKEN_OPERATOR);
	return (TOKEN_WORD);
}

t_token	*new_token(t_mini *mini, t_token_type type, char *word)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (token == NULL)
		system_error(mini);
	token->type = type;
	token->word = word;
	token->next = NULL;
	return (token);
}

t_token	*split_token(t_mini *mini, char **line)
{
	t_token	*token;
	char	*word;
	char	*start;

	start = *line;
	while (**line && !is_metachar(**line))
	{
		if (is_quote(**line))
			in_quote(mini, line);
		else
			(*line)++;
	}
	word = ft_strndup(mini, start, *line - start);
	if (word == NULL)
		system_error(mini);
	token = new_token(mini, get_token_type(word), word);
	if (token == NULL)
	{
		free(word);
		system_error(mini);
	}
	return (token);
}

t_token	*tokenize(t_mini *mini, char *line)
{
	t_token	*head;
	t_token	*token;

	head = NULL;
	while (*line)
	{
		while (*line == ' ' || *line == '\t')
			line++;
		if (*line && is_metachar(*line))
		{
			token = split_token_op(mini, &line);
			add_token_to_list(&head, token);
		}
		else if (*line && !is_metachar(*line))
		{
			token = split_token(mini, &line);
			add_token_to_list(&head, token);
		}
	}
	token = new_token(mini, TOKEN_EOF, NULL);
	add_token_to_list(&head, token);
	return (head);
}
