/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:37:06 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/16 14:33:47 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	write_heredoc_to_tmpfile(t_redirect *redirect)
{
	int		tmpfile_fd;
	char	*line;

	unlink(HEREDOC_TMPFILE);
	tmpfile_fd = open(HEREDOC_TMPFILE, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (tmpfile_fd == -1)
	{
		perror("heredoc: failed to create temporary file");
		exit(1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL || ft_strcmp(line, redirect->file_name) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, tmpfile_fd);
		free(line);
	}
	close(tmpfile_fd);
}

static void	handle_heredoc_child(t_redirect *redirect)
{
	int	exit_status;

	exit_status = 0;
	setup_heredoc_signal_handlers();
	write_heredoc_to_tmpfile(redirect);
	exit(exit_status);
}

static int	handle_heredoc_parent(t_mini *mini, pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			mini->status = 130;
		else if (WTERMSIG(status) == SIGQUIT)
			mini->status = 131;
		return (-1);
	}
	return (WEXITSTATUS(status));
}

static int	setup_heredoc_stdin(t_mini *mini, t_redirect *redirect)
{
	int	tmpfile_fd;

	tmpfile_fd = open(HEREDOC_TMPFILE, O_RDONLY);
	if (tmpfile_fd == -1)
	{
		perror("heredoc: failed to open temporary file");
		exit(1);
	}
	if (dup2(tmpfile_fd, STDIN_FILENO) == -1)
	{
		perror("heredoc: failed to redirect standard input");
		close(tmpfile_fd);
		exit(1);
	}
	close(tmpfile_fd);
	redirect->file_name = ft_strdup(HEREDOC_TMPFILE);
	if (!redirect->file_name)
		system_error(mini);
	return (0);
}

int	handle_heredoc(t_mini *mini, t_redirect *redirect)
{
	pid_t	pid;
	int		exit_status;

	pid = fork();
	if (pid == -1)
		system_error(mini);
	else if (pid == 0)
		handle_heredoc_child(redirect);
	else
	{
		exit_status = handle_heredoc_parent(mini, pid);
		if (exit_status != 0)
			return (-1);
		return (setup_heredoc_stdin(mini, redirect));
	}
	return (0);
}
