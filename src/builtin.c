/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:52 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/11 11:48:41 by mkaihori         ###   ########.fr       */
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

static int	builtin_echo(t_mini *mini, char **argv)
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
	return (mini->status);
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
			print_error(mini, " too many arguments");
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

static bool	is_valid_identifier(t_mini *mini, const char *str)
{
	char	*equals_pos;
	char	*name;
	int		i;

	if (!str || !*str)
		return (false);
	equals_pos = ft_strchr(str, '=');
	if (equals_pos)
		name = ft_strndup(mini, str, equals_pos - str);
	else
	{
		name = ft_strdup(str);
		if (!name)
			system_error(mini);
	}
	if (!ft_isalpha(name[0]) && name[0] != '_')
	{
		free(name);
		return (false);
	}
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			free(name);
			return (false);
		}
		i++;
	}
	free(name);
	return (true);
}


static int	builtin_export(t_mini *mini, char **argv, t_env **env_list)
{
	char	*equals_pos;
	char	*name;
	char	*value;
	int		status;
	int		i;

	if (argv[1] == NULL)
	{
		print_env_list(*env_list);
		return (0);
	}

	status = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(mini, argv[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			status = 1;
			i++;
			continue;
		}

		equals_pos = ft_strchr(argv[i], '=');
		if (!equals_pos)
		{
			set_env_value(mini, env_list, argv[i], "");
		}
		else
		{
			name = ft_strndup(mini, argv[i], equals_pos - argv[i]);
			if (!name)
				return (1);
			value = equals_pos + 1;
			set_env_value(mini, env_list, name, value);
			free(name);
		}
		i++;
	}
	return (status);
}

static int	builtin_unset(char **argv, t_env **env_list)
{
	if (argv[1] == NULL)
		return (0);
	unset_env_value(env_list, argv[1]);
	return (0);
}


static int	builtin_env(char **argv, t_env **env_list)
{
	(void)argv;
	print_env_list(*env_list);
	return (0);
}

static int	builtin_exit(t_mini *mini, char **argv)
{
	int	exit_status;

	if (argv[1] == NULL)
		exit(0);
	if (ft_isnumber(argv[1]) == false)
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			argv[1]);
		exit(2);
	}
	if (argv[2] != NULL)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		mini->status = 1;
		return (1);
	}
	exit_status = ft_atoi(argv[1]);
	exit(exit_status);
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

int	execute_builtin(t_mini *mini, t_builtin_type type, char **argv, t_env **env_list)
{
	if (type == BUILTIN_ECHO)
		return (builtin_echo(mini, argv));
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
