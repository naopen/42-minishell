/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:53:40 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 13:54:56 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env(t_mini *mini, char *env_name)
{
	t_env	*tmp;

	tmp = mini->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, env_name) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	unset_env_value(t_env **env_list, const char *name)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env_list;
	while (curr)
	{
		if (ft_strcmp(curr->name, name) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_list = curr->next;
			free(curr->name);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	print_env_list(t_env *env_list, int flag)
{
	while (env_list)
	{
		if (flag)
		{
			printf("declare -x ");
			printf("%s=\"%s\"\n", env_list->name, env_list->value);
		}
		else
			printf("%s=%s\n", env_list->name, env_list->value);
		env_list = env_list->next;
	}
}
