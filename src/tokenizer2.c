/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:28:04 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 11:29:37 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_opsize(t_mini *mini, char **line)
{
	if (ft_strncmp(*line, "||", 2) == 0 || ft_strncmp(*line, "<<<", 3) == 0)
		custom_error(mini, "Not implement", 1);
	else if (ft_strncmp(*line, "|", 1) == 0)
		return (1);
	else if (ft_strncmp(*line, "<<", 2) == 0)
		return (2);
	else if (ft_strncmp(*line, ">>", 2) == 0)
		return (2);
	else if (ft_strncmp(*line, "<", 1) == 0)
		return (1);
	else if (ft_strncmp(*line, ">", 1) == 0)
		return (1);
	else
		custom_error(mini, "metachar error", 1);
	exit(0);
}

t_token	*split_token_op(t_mini *mini, char **line)
{
	t_token	*token;
	char	*word;
	int		op_size;

	op_size = get_opsize(mini, line);
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

void	add_token_to_list(t_token **head, t_token *token)
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
