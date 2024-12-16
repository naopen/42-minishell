/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:33 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 14:11:15 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/stat.h>

static char	*find_executable(const char *cmd, t_env *env_list)
{
	char	*path_env;
	char	*path;
	char	*dir;
	char	*exec_path;
	char	*tmp;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	path = ft_strdup(path_env);
	dir = path;
	while (dir && *dir)
	{
		tmp = ft_strchr(dir, ':');
		if (tmp)
			*tmp = '\0';
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
		exec_path = tmp;
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

void	execute_external(t_mini *mini, char **argv, t_env *env_list,
		int *status)
{
	pid_t		pid;
	char		**envp;
	char		*exec_path;
	struct stat	st;
	int			execve_ret;

	envp = NULL;
	exec_path = find_executable(argv[0], env_list);
	if (!exec_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", argv[0]);
		*status = 127;
		return ;
	}
	pid = fork();
	if (pid == -1)
		system_error(mini);
	if (!pid)
	{
		envp = env_to_envp(mini, env_list);
		execve_ret = execve(exec_path, argv, envp);
		if (execve_ret == -1)
		{
			if (errno == ENOENT)
			{
				if (stat(exec_path, &st) == 0 && S_ISDIR(st.st_mode))
				{
					fprintf(stderr, "minishell: %s: Is a directory\n", argv[0]);
					exit(126);
				}
				else
				{
					if (ft_strchr(argv[0], '/'))
						fprintf(stderr,
							"minishell: %s:\
							No such file or directory\n",
							argv[0]);
					else
						fprintf(stderr,
							"minishell: %s:\
							command not found\n",
							argv[0]);
					exit(127);
				}
			}
			else if (errno == EACCES)
			{
				fprintf(stderr, "minishell: %s: Permission denied\n", argv[0]);
				exit(126);
			}
			else
			{
				perror("minishell");
				exit(126);
			}
		}
	}
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
	{
		*status = WEXITSTATUS(*status);
		mini->status = *status;
	}
	else if (WIFSIGNALED(*status))
	{
		*status = 128 + WTERMSIG(*status);
		mini->status = *status;
	}
	free(exec_path);
	return ;
}

void	execute_command(t_mini *mini, t_node *node, t_env *env_list,
		int *status)
{
	t_builtin_type	builtin_type;
	int				ret;
	int				i;

	if (node == NULL)
		return ;
	i = 0;
	ret = do_redirection(mini, node->redirects);
	if (ret != 0)
	{
		*status = mini->status;
		return ;
	}
	while (node->argv[i] && node->argv[i][0] == '\0')
		i++;
	builtin_type = get_builtin_type(node->argv[i]);
	if (builtin_type != BUILTIN_UNKNOWN)
		mini->status = execute_builtin(mini, builtin_type, node->argv + i,
				&env_list);
	else if (node->argv[i])
	{
		execute_external(mini, node->argv + i, env_list, status);
		mini->status = *status;
	}
	else if (!node->argv[i])
	{
		print_error(mini, "bash: : command not found\n", NULL);
		mini->status = 127;
	}
	return ;
}

t_node	*process_command(t_node *node, int p_num)
{
	int	i;

	i = 0;
	while (i < p_num)
	{
		node = node->next;
		i++;
	}
	return (node);
}

void	execute(t_mini *mini)
{
	mini->process = count_node(mini->node);
	if (mini->process > 1)
	{
		mini->pipefd = malloc(sizeof(int [2]) * (mini->process - 1));
		if (!mini->pipefd)
			system_error(mini);
	}
	mini->pid = malloc(sizeof(pid_t) * mini->process);
	if (!mini->pid)
		system_error(mini);
	execute_pipeline(mini, mini->node, mini->process);
	if (dup2(mini->backup_in, STDIN_FILENO) == -1)
		system_error(mini);
	if (dup2(mini->backup_out, STDOUT_FILENO) == -1)
		system_error(mini);
	return ;
}
