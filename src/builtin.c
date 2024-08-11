/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:52 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/11 16:23:19 by nkannan          ###   ########.fr       */
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
        // 空白文字の出力修正
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");

    // 終了ステータスを0に設定
    return (0);
}

int	builtin_cd(char **argv)
{
	if (argv[1] == NULL)
	{
		char	*home;

		home = getenv("HOME");
		if (home == NULL)
		{
			printf("minishell: cd: HOME not set\n");
			return (1);
		}
		if (chdir(home) == -1)
		{
			perror("minishell: cd");
			return (1);
		}
	}
	else
	{
		if (chdir(argv[1]) == -1)
		{
			perror("minishell: cd");
			return (1);
		}
	}
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
		exit_status = ft_atoi(argv[1]);
		if (exit_status < 0 || exit_status > 255)
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
		if (ft_strchr(argv[i], '=') == NULL)
		{
			printf("minishell: export: `%s': not a valid identifier\n", argv[i]);
			return (1);
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
