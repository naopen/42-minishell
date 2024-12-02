/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 17:09:19 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/02 16:10:43 by mkaihori         ###   ########.fr       */
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
	if (ft_strchr(";|", *str))
		return (TOKEN_OPERATOR);
	return (TOKEN_WORD);
}

static t_token	*new_token(t_mini *mini, t_token_type type, char *word)
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

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

static t_token	*split_token(t_mini *mini, char **line)
{
	t_token	*token;
	char	*word;
	char	*start;
	char	quote;

	start = *line;
	while (**line && !is_metachar(**line))
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
				custum_error(mini, "minishell: syntax error: unclosed quote", 1);
		}
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

static t_token	*split_token_op(t_mini *mini, char **line)
{
	t_token	*token;
	char	*word;
	int		op_size;

	op_size = 0;
	if (ft_strncmp(*line, "||", 2) == 0 || ft_strncmp(*line, "<<<", 3) == 0)
		custum_error(mini, "Not implement", 1);
	else if (ft_strncmp(*line, "|", 1) == 0)
		op_size = 1;
	else if (ft_strncmp(*line, "<<", 2) == 0)
		op_size = 2;
	else if (ft_strncmp(*line, ">>", 2) == 0)
		op_size = 2;
	else if (ft_strncmp(*line, "<", 1) == 0)
		op_size = 1;
	else if (ft_strncmp(*line, ">", 1) == 0)
		op_size = 1;
	else
		custum_error(mini, "metachar error", 1);
	word = ft_strndup(mini, *line, op_size);
	if (word == NULL)
		system_error(mini);
	(*line) += op_size;
	token = new_token(mini, get_token_type(word), word);
	if (token == NULL)
	{
		free(word);
		system_error(mini);
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

t_token	*tokenize(t_mini *mini, char *line)
{
	t_token	*head;
	t_token	*token;

	head = NULL;
	while (*line)
	{
		while (*line == ' ' || *line == '\t')
			line++;
		if (is_metachar(*line))
		{
			token = split_token_op(mini, &line);
			add_token_to_list(&head, token);
		}
		else if (!is_metachar(*line))
		{
			token = split_token(mini, &line);
			add_token_to_list(&head, token);
		}
	}
	token = new_token(mini, TOKEN_EOF, NULL);
	add_token_to_list(&head, token);
	return (head);
}
