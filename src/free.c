/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:53:17 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/02 18:29:14 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_strs(char **str)
{
	int	i;

	if (str)
	{
		i = 0;
		while (str[i])
		{
			free(str[i]);
			i++;
		}
		free(str);
	}
	return ;
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->name);
		free(env->value);
		free(env);
		env = tmp;
	}
	return ;
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
	return ;
}

void	free_node(t_node *node)
{
	t_node		*tmp_node;
	t_redirect	*tmp_redirect;

	while (node)
	{
		tmp_node = node->next;
		free_strs(node->argv);
		while (node->redirects)
		{
			tmp_redirect = node->redirects->next;
			free(node->redirects->file_name);
			free(node->redirects);
			node->redirects = tmp_redirect;
		}
		free(node);
		node = tmp_node;
	}
	return ;
}
