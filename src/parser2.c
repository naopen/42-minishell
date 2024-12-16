/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:35:18 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 11:37:36 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**get_command_args(t_mini *mini, t_token **token_list, int argc)
{
	char	**strs;
	t_token	*token;

	token = *token_list;
	strs = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!strs)
		system_error(mini);
	argc = 0;
	while (token && token->type != TOKEN_OPERATOR && token->type != TOKEN_EOF)
	{
		if (token->type != TOKEN_WORD)
			token = token->next->next;
		else
		{
			strs[argc++] = expand_env_var(mini, ft_strdup(token->word));
			token = token->next;
		}
	}
	strs[argc] = NULL;
	*token_list = token;
	return (strs);
}

void	add_redirect_to_list(t_redirect **head, t_redirect *redirect)
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

char	*cut_quote(t_mini *mini, char *word, int single_q, int double_q)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = (char *)malloc(sizeof(char) * (ft_strlen(word) + 1));
	if (!new)
	{
		free(word);
		system_error(mini);
	}
	while (word[i] != '\0')
	{
		if (word[i] == '\'' && !double_q)
			single_q = !single_q;
		else if (word[i] == '\"' && !single_q)
			double_q = !double_q;
		else
			new[j++] = word[i];
		i++;
	}
	new[j] = '\0';
	free(word);
	return (new);
}
