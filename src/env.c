/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:09:10 by nkannan           #+#    #+#             */
/*   Updated: 2024/11/16 16:00:09 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*create_env_list(char **environ)
{
	t_env	*head;
	t_env	*env;
	char	*name;
	char	*value;

	head = NULL;
	while (*environ)
	{
		name = ft_strndup(*environ, ft_strchr(*environ, '=') - *environ);
		value = ft_strdup(ft_strchr(*environ, '=') + 1);
		env = (t_env *)malloc(sizeof(t_env));
		if (env == NULL)
			exit_with_error("minishell: malloc error");
		env->name = name;
		env->value = value;
		env->next = head;
		head = env;
		environ++;
	}
	return (head);
}

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

int	set_env_value(t_env **env_list, const char *name, const char *value)
{
	t_env	*env;

	env = *env_list;
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			if (env->value == NULL)
				exit_with_error("minishell: malloc error");
			return (0);
		}
		env = env->next;
	}
	env = (t_env *)malloc(sizeof(t_env));
	if (env == NULL)
		exit_with_error("minishell: malloc error");
	env->name = ft_strdup(name);
	env->value = ft_strdup(value);
	if (env->name == NULL || env->value == NULL)
	{
		free(env->name);
		free(env->value);
		exit_with_error("minishell: malloc error");
	}
	env->next = *env_list;
	*env_list = env;
	return (0);
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

void	print_env_list(t_env *env_list)
{
	while (env_list)
	{
		printf("%s=%s\n", env_list->name, env_list->value);
		env_list = env_list->next;
	}
}

void	free_env_list(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list->next;
		free(env_list->name);
		free(env_list->value);
		free(env_list);
		env_list = tmp;
	}
}

char	**env_to_envp(t_env *env_list)
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
		exit_with_error("minishell: malloc error");
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
