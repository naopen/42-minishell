/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:26:23 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 15:05:54 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static bool	check_identifier_chars(const char *name)
{
	int	i;

	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

bool	is_valid_identifier(t_mini *mini, const char *str)
{
	char	*equals_pos;
	char	*name;
	bool	result;

	result = false;
	if (!str || !*str)
		return (false);
	equals_pos = ft_strchr(str, '=');
	name = NULL;
	if (equals_pos)
		name = ft_strndup(mini, str, equals_pos - str);
	else
		name = ft_strdup(str);
	if (!name)
		system_error(mini);
	if (ft_isalpha(name[0]) || name[0] == '_')
		result = check_identifier_chars(name);
	free(name);
	return (result);
}

static void	export_single_variable(t_mini *mini, char *arg)
{
	char	*equals_pos;
	char	*name;
	char	*value;

	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
		set_env_value(mini, arg, "");
	else
	{
		name = ft_strndup(mini, arg, equals_pos - arg);
		if (!name)
			system_error(mini);
		value = equals_pos + 1;
		set_env_value(mini, name, value);
		free(name);
	}
}

int	builtin_export(t_mini *mini, char **argv, t_env **env_list)
{
	int	status;
	int	i;

	(void)env_list;
	if (argv[1] == NULL)
	{
		print_env_list(mini->env, 1);
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
		}
		else
			export_single_variable(mini, argv[i]);
		i++;
	}
	return (status);
}
