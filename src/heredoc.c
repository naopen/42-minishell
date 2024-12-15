/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:37:06 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/15 18:53:59 by nkannan          ###   ########.fr       */
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

int	handle_heredoc(t_mini *mini, t_redirect *redirect)
{
	pid_t	pid;
	int		status;
	int		tmpfile_fd;

	pid = fork();
	if (pid == -1)
		system_error(mini);
	else if (pid == 0) // 子プロセス
	{
		setup_heredoc_signal_handlers();
		write_heredoc_to_tmpfile(redirect);
		exit(0);
	}
	else // 親プロセス
	{
		waitpid(pid, &status, 0); // 子プロセスの終了を待つ
		if (WIFSIGNALED(status))  // シグナルで終了した場合
		{
			if (WTERMSIG(status) == SIGINT) // SIGINT を検出
			{
				mini->status = 130;
				return (-1);
			}
			else if (WTERMSIG(status) == SIGQUIT) // SIGQUIT を無視
			{
				mini->status = 131;
				return (-1);
			}
		}
		if (WEXITSTATUS(status) != 0)
			return (-1);
		// temporary file を開く
		tmpfile_fd = open(HEREDOC_TMPFILE, O_RDONLY);
		if (tmpfile_fd == -1)
		{
			perror("heredoc: failed to open temporary file");
			exit(1);
		}

		// 標準入力にリダイレクト
		if (dup2(tmpfile_fd, STDIN_FILENO) == -1)
		{
			perror("heredoc: failed to redirect standard input");
			close(tmpfile_fd);
			exit(1);
		}
		close(tmpfile_fd);

		// リダイレクト先をテンポラリファイルに設定
		redirect->file_name = ft_strdup(HEREDOC_TMPFILE);
		if (!redirect->file_name)
			system_error(mini);

		return (0);
	}
	return (0);
}
