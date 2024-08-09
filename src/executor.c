/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:33 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/08 17:42:23 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_pipeline(t_command *cmd, t_env **env_head)
{
	int		pipefd[2];
	int		prev_pipefd[2];
	pid_t	pid;
	int		status;

	prev_pipefd[0] = -1;
	prev_pipefd[1] = -1;
	while (cmd)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
		pid = execute_command(cmd, pipefd, prev_pipefd, env_head);
		// 子プロセスの終了を待つ
		if (waitpid(pid, &status, 0) == -1)
			fatal_error("waitpid");
		if (prev_pipefd[0] != -1)
			close_pipe(prev_pipefd);
		prev_pipefd[0] = pipefd[0];
		prev_pipefd[1] = pipefd[1];
		cmd = cmd->next;
	}
	// 最後のコマンドの終了ステータスを返す
	return (WEXITSTATUS(status));
}

pid_t	execute_command(t_command *cmd, int pipefd[2], int prev_pipefd[2],
		t_env **env_head)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
	{
		prepare_pipe(cmd, pipefd, prev_pipefd);
		if (handle_redirections(cmd) == -1)
			exit(EXIT_FAILURE);
		if (is_builtin(cmd->argv[0]))
			exit(execute_builtin(cmd->argv[0], cmd->argv, env_head));
		path = find_command_path(cmd->argv[0]);
		if (path == NULL)
		{
			printf("minishell: command not found: %s\n", cmd->argv[0]);
			exit(127);
		}
		if (execve(path, cmd->argv, create_environ(*env_head)) == -1)
			fatal_error("execve");
	}
	return (pid);
}

char	*find_command_path(char *command)
{
	char	*path_env;
	char	*path;
	char	*dir;
	size_t	len;

	path_env = getenv("PATH");
	if (path_env == NULL)
		return (NULL);
	path = ft_strdup(path_env);
	if (path == NULL)
		fatal_error("malloc");
	dir = ft_strtok(path, ":");
	while (dir != NULL)
	{
		len = ft_strlen(dir) + ft_strlen(command) + 2;
		char *full_path = malloc(len);
		if (full_path == NULL)
			fatal_error("malloc");
		ft_strlcpy(full_path, dir, len);
		ft_strlcat(full_path, "/", len);
		ft_strlcat(full_path, command, len);
		if (access(full_path, X_OK) == 0)
		{
			free(path);
			return (full_path);
		}
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}

void	close_pipe(int pipefd[2])
{
	if (close(pipefd[0]) == -1)
		fatal_error("close");
	if (close(pipefd[1]) == -1)
		fatal_error("close");
}

void	prepare_pipe(t_command *cmd, int pipefd[2], int prev_pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
			fatal_error("pipe");
	}
	if (prev_pipefd[0] != -1)
	{
		dup2(prev_pipefd[0], STDIN_FILENO);
		close_pipe(prev_pipefd);
	}
	if (pipefd[1] != -1)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

int	is_builtin(char *command)
{
	return (ft_strcmp(command, "echo") == 0 || ft_strcmp(command, "cd") == 0 ||
			ft_strcmp(command, "pwd") == 0 || ft_strcmp(command, "exit") == 0
			|| ft_strcmp(command, "export") == 0
			|| ft_strcmp(command, "unset") == 0
			|| ft_strcmp(command, "env") == 0);
}

int	execute_builtin(char *command, char **argv, t_env **env_head)
{
	if (ft_strcmp(command, "echo") == 0)
		return (builtin_echo(argv));
	else if (ft_strcmp(command, "cd") == 0)
		return (builtin_cd(argv));
	else if (ft_strcmp(command, "pwd") == 0)
		return (builtin_pwd(argv));
	else if (ft_strcmp(command, "exit") == 0)
		return (builtin_exit(argv));
	else if (ft_strcmp(command, "export") == 0)
		return (builtin_export(argv, env_head));
	else if (ft_strcmp(command, "unset") == 0)
		return (builtin_unset(argv, env_head));
	else if (ft_strcmp(command, "env") == 0)
		return (builtin_env(argv, *env_head));
	return (1);
}
