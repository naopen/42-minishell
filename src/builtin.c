/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:52 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/12 01:27:39 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_cd(char **argv)
{
	char	*path;

	if (argv[1] == NULL)
	{
		char	*home;

		home = getenv("HOME");
		if (home == NULL)
		{
			printf("minishell: cd: HOME not set\n");
			return (1);
		}
		path = home;
	}
	else
		path = expand_variable(argv[1], 0); // 環境変数を展開
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		if (argv[1] != NULL)
			free(path);
		return (1);
	}
	if (argv[1] != NULL)
		free(path);
	return (0);
}

int	builtin_pwd(char **argv)
{
	char	cwd[PATH_MAX];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

int	builtin_exit(char **argv)
{
	int	exit_status;

	exit_status = 0;
	if (argv[1])
	{
		if (!ft_isinteger(argv[1]))
		{
			printf("minishell: exit: numeric argument required\n");
			return (255);
		}
		exit_status = ft_atoi(argv[1]);
		if (exit_status < 0 || exit_status > 255) // exit status が 0 ~ 255 の範囲外の場合の処理を追加
		{
			printf("minishell: exit: numeric argument required\n");
			return (255);
		}
	}
	exit(exit_status);
}

int	builtin_export(char **argv, t_env **env_head)
{
	t_env	*env;
	char	*key;
	char	*value;
	int		i;

	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i])) // 識別子のバリデーション
		{
			printf("minishell: export: `%s': not a valid identifier\n", argv[i]);
			return (1); // bashと同じexit status
		}
		key = ft_strndup(argv[i], ft_strchr(argv[i], '=') - argv[i]);
		if (key == NULL)
			fatal_error("malloc");
		value = ft_strdup(ft_strchr(argv[i], '=') + 1);
		if (value == NULL)
			fatal_error("malloc");
		env = search_env(*env_head, key);
		if (env)
		{
			free(env->value);
			env->value = value;
		}
		else
		{
			env = (t_env *)malloc(sizeof(t_env));
			if (env == NULL)
				fatal_error("malloc");
			env->key = key;
			env->value = value;
			env->next = NULL;
			append_env(env_head, env);
		}
		i++;
	}
	return (0);
}

int	builtin_unset(char **argv, t_env **env_head)
{
	int		i;
	t_env	*env;
	t_env	*prev;

	i = 1;
	while (argv[i])
	{
		env = *env_head;
		prev = NULL;
		while (env)
		{
			if (ft_strcmp(env->key, argv[i]) == 0)
			{
				if (prev)
					prev->next = env->next;
				else
					*env_head = env->next;
				free(env->key);
				free(env->value);
				free(env);
				break ;
			}
			prev = env;
			env = env->next;
		}
		i++;
	}
	return (0);
}

int	builtin_env(char **argv, t_env *env)
{
	(void)argv;
	print_env(env);
	return (0);
}
