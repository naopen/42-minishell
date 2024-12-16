/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:09:10 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 15:18:10 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env_value(t_env *env_list, const char *name)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->name, name) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

static void	handle_malloc_error(t_mini *mini, t_env *env)
{
	free(env->name);
	free(env->value);
	system_error(mini);
}

int	set_env_value(t_mini *mini, const char *name, const char *value)
{
	t_env	*env;

	env = mini->env;
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			if (!env->value)
				system_error(mini);
			return (0);
		}
		env = env->next;
	}
	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		system_error(mini);
	env->name = ft_strdup(name);
	env->value = ft_strdup(value);
	if (!env->name || !env->value)
		handle_malloc_error(mini, env);
	env->next = mini->env;
	mini->env = env;
	return (0);
}

char	**env_to_envp(t_mini *mini, t_env *env_list)
{
	char	**envp;
	t_env	*env;
	int		i;

	i = 0;
	env = env_list;
	while (env)
	{
		i++;
		env = env->next;
	}
	envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (envp == NULL)
		system_error(mini);
	i = 0;
	env = env_list;
	while (env)
	{
		envp[i] = ft_strjoin(env->name, "=");
		envp[i] = ft_strjoin(envp[i], env->value);
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}
