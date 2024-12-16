/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:38:23 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 15:38:53 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*join_path_cmd(char *dir, const char *cmd, char *path)
{
	char	*exec_path;
	char	*tmp;

	exec_path = ft_strjoin(dir, "/");
	if (!exec_path)
	{
		free(path);
		return (NULL);
	}
	tmp = ft_strjoin(exec_path, cmd);
	free(exec_path);
	if (!tmp)
	{
		free(path);
		return (NULL);
	}
	return (tmp);
}

static char	*search_path(const char *cmd, char *path)
{
	char	*dir;
	char	*tmp;
	char	*exec_path;

	dir = path;
	while (dir && *dir)
	{
		tmp = ft_strchr(dir, ':');
		if (tmp)
			*tmp = '\0';
		exec_path = join_path_cmd(dir, cmd, path);
		if (!exec_path)
			return (NULL);
		if (access(exec_path, X_OK) == 0)
		{
			free(path);
			return (exec_path);
		}
		free(exec_path);
		if (!tmp || !ft_strchr(dir + ft_strlen(dir) + 1, ':'))
			break ;
		dir = dir + ft_strlen(dir) + 1;
	}
	free(path);
	return (NULL);
}

char	*find_executable(const char *cmd, t_env *env_list)
{
	char	*path_env;
	char	*path;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	path = ft_strdup(path_env);
	return (search_path(cmd, path));
}
