/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:16:26 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 15:18:14 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_env_list(t_env **head, t_env *new)
{
	t_env	*tmp;

	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return ;
}

t_env	*create_env_list(t_mini *mini, char **environ)
{
	t_env	*head;
	t_env	*env;
	char	*name;
	char	*value;

	head = NULL;
	while (*environ)
	{
		name = ft_strndup(mini, *environ, ft_strchr(*environ, '=') - *environ);
		value = ft_strdup(ft_strchr(*environ, '=') + 1);
		env = (t_env *)malloc(sizeof(t_env));
		if (env == NULL)
			system_error(mini);
		env->name = name;
		env->value = value;
		env->next = NULL;
		add_env_list(&head, env);
		environ++;
	}
	return (head);
}
