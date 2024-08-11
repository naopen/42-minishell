/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:09:10 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/11 17:08:04 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**create_environ(t_env *env_head)
{
	int		i;
	t_env	*env;
	char	**environ;

	i = 0;
	env = env_head;
	while (env)
	{
		env = env->next;
		i++;
	}
	environ = (char **)malloc(sizeof(char *) * (i + 1));
	if (environ == NULL)
		fatal_error("malloc");
	i = 0;
	env = env_head;
	while (env)
	{
		environ[i] = ft_strjoin(env->key, "=");
		if (environ[i] == NULL)
			fatal_error("malloc");
		environ[i] = ft_strjoin_free(environ[i], env->value);
		env = env->next;
		i++;
	}
	environ[i] = NULL;
	return (environ);
}

t_env	*create_env_list(char **environ)
{
	t_env	*head;
	t_env	*env;
	int		i;

	head = NULL;
	i = 0;
	while (environ[i])
	{
		env = (t_env *)malloc(sizeof(t_env));
		if (env == NULL)
			fatal_error("malloc");
		env->key = ft_strndup(environ[i], ft_strchr(environ[i], '=') - environ[i]);
		if (env->key == NULL)
			fatal_error("malloc");
		env->value = ft_strdup(ft_strchr(environ[i], '=') + 1);
		if (env->value == NULL)
			fatal_error("malloc");
		env->next = NULL;
		append_env(&head, env);
		i++;
	}
	return (head);
}

void	print_env(t_env *env)
{
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

t_env	*search_env(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	append_env(t_env **head, t_env *env)
{
	t_env	*tmp;

	if (*head == NULL)
	{
		*head = env;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = env;
}
