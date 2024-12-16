/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:52 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 13:48:34 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static bool	is_n_option(const char *str)
{
	int	i;

	i = 0;
	if (ft_strchr(str, '\"') || ft_strchr(str, '\''))
		return (false);
	if (str[i++] != '-')
		return (false);
	if (str[i++] != 'n')
		return (false);
	while (str[i] == 'n')
		i++;
	if (str[i] != '\0')
		return (false);
	return (true);
}

static int	builtin_echo(char **argv)
{
	int		i;
	bool	newline;

	newline = true;
	i = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		newline = false;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1] && argv[i][0] != '\0')
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

static int	builtin_cd(t_mini *mini, char **argv)
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
		if (argv[2] != NULL)
		{
			print_error(mini, " too many arguments", NULL);
			return (1);
		}
		if (chdir(argv[1]) == -1)
		{
			perror("cd");
			return (1);
		}
	}
	return (0);
}

t_builtin_type	get_builtin_type(const char *cmd)
{
	if (!cmd)
		return (BUILTIN_UNKNOWN);
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

int	execute_builtin(t_mini *mini, t_builtin_type type, char **argv,
		t_env **env_list)
{
	if (type == BUILTIN_ECHO)
		return (builtin_echo(argv));
	if (type == BUILTIN_CD)
		return (builtin_cd(mini, argv));
	if (type == BUILTIN_PWD)
		return (builtin_pwd(argv));
	if (type == BUILTIN_EXPORT)
		return (builtin_export(mini, argv, env_list));
	if (type == BUILTIN_UNSET)
		return (builtin_unset(argv, env_list));
	if (type == BUILTIN_ENV)
		return (builtin_env(argv, env_list));
	if (type == BUILTIN_EXIT)
		return (builtin_exit(mini, argv));
	return (1);
}
