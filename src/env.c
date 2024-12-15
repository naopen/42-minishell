/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:09:10 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/15 17:41:24 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_env_list(t_env **head, t_env *new)
{
	t_env *tmp;

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
			if (env->value == NULL)
				system_error(mini);
			return (0);
		}
		env = env->next;
	}
	env = (t_env *)malloc(sizeof(t_env));
	if (env == NULL)
		system_error(mini);
	env->name = ft_strdup(name);
	env->value = ft_strdup(value);
	if (env->name == NULL || env->value == NULL)
	{
		free(env->name);
		free(env->value);
		system_error(mini);
	}
	env->next = mini->env;
	mini->env = env;
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
		printf("declare -x ");
		printf("%s=\"%s\"\n", env_list->name, env_list->value);
		env_list = env_list->next;
	}
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
