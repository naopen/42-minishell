/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:49:32 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/16 12:15:11 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_metachar(char c)
{
	if (ft_strchr("|&;()<> \t", c))
		return (true);
	return (false);
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

void	in_quote(t_mini *mini, char **line)
{
	char	quote;

	quote = **line;
	(*line)++;
	while (**line && **line != quote)
		(*line)++;
	if (**line == quote)
		(*line)++;
	else
		custom_error(mini, "minishell: syntax error: unclosed quote", 1);
	return ;
}
