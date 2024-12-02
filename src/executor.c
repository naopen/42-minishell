/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:33 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/02 16:44:38 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char *find_executable(const char *cmd, t_env *env_list)
{
    char *path_env;
    char *path;
    char *dir;
    char *exec_path;
    char *tmp;

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
            break;
        dir = dir + ft_strlen(dir) + 1;
    }
    free(path);
    return (NULL);
}

void	execute_external(t_mini *mini, char **argv, t_env *env_list, int *status)
{
	pid_t	pid;
	char	**envp;
	char    *exec_path;

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
		if (execve(exec_path, argv, envp) == -1)
		{
			system_error(mini);
			free(exec_path);
			system_error(mini);
		}
	}
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	return ;
}

void	execute_command(t_mini *mini, t_node *node, t_env *env_list, int * status)
{
	t_builtin_type	builtin_type;

	if (node == NULL)
		return ;

	do_redirection(mini, node->redirects);
	builtin_type = get_builtin_type(node->argv[0]);
	if (builtin_type != BUILTIN_UNKNOWN)
		execute_builtin(mini, builtin_type, node->argv, &env_list, status);
	else
		execute_external(mini, node->argv, env_list, status);
	return ;
}

void	child_process(t_mini *mini, t_node *node, t_env *env_list, int *status, int pipefd[2])
{
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		system_error(mini);
	close(pipefd[1]);
	execute_command(mini, node, env_list, status);
	exit (*status);
}

void	execute_pipeline(t_mini *mini, t_node *node, t_env *env_list, int *status)
{
	int		pipefd[2];
	pid_t	parent;

	if (node->next == NULL)
		return (execute_command(mini, node, env_list, status));
	if (pipe(pipefd) == -1)
		system_error(mini);
	parent = fork();
	if (parent == -1)
		system_error(mini);
	if (!parent)
		child_process(mini, node, env_list, status, pipefd);
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		system_error(mini);
	close(pipefd[0]);
	waitpid(parent, status, 0);
	execute_pipeline(mini, node->next, env_list, status);
	return ;
}

void	execute(t_mini *mini)
{
	execute_pipeline(mini, mini->node, mini->env, &(mini->status));
	if (dup2(mini->backup_in, STDIN_FILENO) == -1)
		system_error(mini);
	if (dup2(mini->backup_out, STDOUT_FILENO) == -1)
		system_error(mini);
	return ;
}
