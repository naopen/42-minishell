/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:33 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/15 17:33:33 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	do_redirection(t_redirect *redirect)
{
	int	fd;

	while (redirect)
	{
		if (redirect->type == REDIRECT_IN)
		{
			fd = open(redirect->file_name, O_RDONLY);
			if (fd == -1)
				exit_with_error("minishell: open error");
			if (dup2(fd, STDIN_FILENO) == -1)
				exit_with_error("minishell: dup2 error");
			close(fd);
		}
		else if (redirect->type == REDIRECT_OUT)
		{
			fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				exit_with_error("minishell: open error");
			if (dup2(fd, STDOUT_FILENO) == -1)
				exit_with_error("minishell: dup2 error");
			close(fd);
		}
		else if (redirect->type == REDIRECT_APPEND)
		{
			fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				exit_with_error("minishell: open error");
			if (dup2(fd, STDOUT_FILENO) == -1)
				exit_with_error("minishell: dup2 error");
			close(fd);
		}
		else if (redirect->type == REDIRECT_HEREDOC)
			handle_heredoc(redirect);
		redirect = redirect->next;
	}
	return (0);
}

int	handle_heredoc(t_redirect *redirect)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	char	*line;

	if (pipe(pipefd) == -1)
		exit_with_error("minishell: pipe error");
	pid = fork();
	if (pid == -1)
		exit_with_error("minishell: fork error");
	if (pid == 0)
	{
		close(pipefd[0]);
		while (1)
		{
			line = readline("> ");
			if (line == NULL || strcmp(line, redirect->file_name) == 0)
				break ;
			ft_putendl_fd(line, pipefd[1]);
			free(line);
		}
		close(pipefd[1]);
		exit(0);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
	}
}

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

static int	execute_external(char **argv, t_redirect *redirects,
		t_env *env_list)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char    *exec_path;

	envp = NULL;
	exec_path = find_executable(argv[0], env_list);
	if (!exec_path)
	{
		fprintf(stderr, "minishell: command not found: %s\n", argv[0]);
		return (127);
	}

	pid = fork();
	if (pid == -1)
		exit_with_error("minishell: fork error");
	if (pid == 0)
	{
		envp = env_to_envp(env_list);
		if (do_redirection(redirects) == 1)
		{
			ft_strarrdel(envp);
			free(exec_path);
			exit(1);
		}
		if (execve(exec_path, argv, envp) == -1)
		{
			ft_strarrdel(envp);
			free(exec_path);
			exit_with_error("minishell: execve error");
		}
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}

static int	execute_command(t_node *node, t_env *env_list)
{
	t_builtin_type	builtin_type;

	if (node == NULL)
		return (0);
	builtin_type = get_builtin_type(node->argv[0]);
	if (builtin_type != BUILTIN_UNKNOWN)
		return (execute_builtin(builtin_type, node->argv, &env_list));
	return (execute_external(node->argv, node->redirects, env_list));
}

static int	execute_pipeline(t_node *node, t_env *env_list)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (node->next == NULL)
		return (execute_command(node, env_list));

	if (pipe(pipefd) == -1)
		exit_with_error("minishell: pipe error");

	pid1 = fork();
	if (pid1 == -1)
		exit_with_error("minishell: fork error");

	if (pid1 == 0)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			exit_with_error("minishell: dup2 error");
		close(pipefd[1]);
		exit(execute_command(node, env_list));
	}

	pid2 = fork();
	if (pid2 == -1)
		exit_with_error("minishell: fork error");

	if (pid2 == 0)
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			exit_with_error("minishell: dup2 error");
		close(pipefd[0]);
		exit(execute_pipeline(node->next, env_list));
	}

	close(pipefd[0]);
	close(pipefd[1]);

	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute(t_node *node, t_env *env_list)
{
	return (execute_pipeline(node, env_list));
}
