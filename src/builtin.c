/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:52 by nkannan           #+#    #+#             */
/*   Updated: 2024/11/16 15:59:50 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	builtin_echo(char **argv)
{
	int		i;
	bool	newline;

	newline = true;
	i = 1;
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		newline = false;
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

static int	builtin_cd(char **argv)
{
	if (argv[1] == NULL)
	{
		if (chdir(getenv("HOME")) == -1)
		{
			perror("cd");
			return (1);
		}
	}
	else
	{
		if (chdir(argv[1]) == -1)
		{
			perror("cd");
			return (1);
		}
	}
	return (0);
}

static int	builtin_pwd(char **argv)
{
	char	cwd[PATH_MAX];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

static int	builtin_export(char **argv, t_env **env_list)
{
	char	*equals_pos;
	char	*name;
	char	*value;

	if (argv[1] == NULL)
	{
		print_env_list(*env_list);
		return (0);
	}

	equals_pos = ft_strchr(argv[1], '=');
	if (!equals_pos)
		return (set_env_value(env_list, argv[1], ""));

	name = ft_strndup(argv[1], equals_pos - argv[1]);
	if (!name)
		return (1);

	value = equals_pos + 1;
	if (set_env_value(env_list, name, value) != 0)
	{
		free(name);
		return (1);
	}

	free(name);
	return (0);
}

static int	builtin_unset(char **argv, t_env **env_list)
{
	if (argv[1] == NULL)
		return (1);
	unset_env_value(env_list, argv[1]);
	return (0);
}

static int	builtin_env(char **argv, t_env **env_list)
{
	(void)argv;
	print_env_list(*env_list);
	return (0);
}

static int	builtin_exit(char **argv)
{
	int	exit_status;

	if (argv[1] == NULL)
		exit(0);
	if (ft_isnumber(argv[1]) == false)
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			argv[1]);
		exit(255);
	}
	exit_status = ft_atoi(argv[1]);
	exit(exit_status);
}

t_builtin_type	get_builtin_type(const char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(cmd, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(cmd, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp(cmd, "unset") == 0)
		return (BUILTIN_UNSET);
	if (ft_strcmp(cmd, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(cmd, "exit") == 0)
		return (BUILTIN_EXIT);
	return (BUILTIN_UNKNOWN);
}

int	execute_builtin(t_builtin_type type, char **argv, t_env **env_list)
{
	if (type == BUILTIN_ECHO)
		return (builtin_echo(argv));
	if (type == BUILTIN_CD)
		return (builtin_cd(argv));
	if (type == BUILTIN_PWD)
		return (builtin_pwd(argv));
	if (type == BUILTIN_EXPORT)
		return (builtin_export(argv, env_list));
	if (type == BUILTIN_UNSET)
		return (builtin_unset(argv, env_list));
	if (type == BUILTIN_ENV)
		return (builtin_env(argv, env_list));
	if (type == BUILTIN_EXIT)
		return (builtin_exit(argv));
	return (1);

}
