/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:26:23 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 13:44:40 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_valid_identifier(t_mini *mini, const char *str)
{
	char	*equals_pos;
	char	*name;
	int		i;

	if (!str || !*str)
		return (false);
	equals_pos = ft_strchr(str, '=');
	if (equals_pos)
	{
		name = ft_strndup(mini, str, equals_pos - str);
		if (!name)
			system_error(mini);
	}
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

int	builtin_export(t_mini *mini, char **argv, t_env **env_list)
{
	char	*equals_pos;
	char	*name;
	char	*value;
	int		status;
	int		i;

	if (argv[1] == NULL)
	{
		print_env_list(*env_list, 1);
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
			continue ;
		}
		equals_pos = ft_strchr(argv[i], '=');
		if (!equals_pos)
			set_env_value(mini, argv[i], "");
		else
		{
			name = ft_strndup(mini, argv[i], equals_pos - argv[i]);
			if (!name)
				system_error(mini);
			value = equals_pos + 1;
			set_env_value(mini, name, value);
			free(name);
		}
		i++;
	}
	return (status);
}
