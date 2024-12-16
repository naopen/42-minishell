/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:45:04 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 13:49:31 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_unset(char **argv, t_env **env_list)
{
	if (argv[1] == NULL)
		return (0);
	unset_env_value(env_list, argv[1]);
	return (0);
}

int	builtin_env(char **argv, t_env **env_list)
{
	(void)argv;
	print_env_list(*env_list, 0);
	return (0);
}

int	builtin_pwd(char **argv)
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

int	builtin_exit(t_mini *mini, char **argv)
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
