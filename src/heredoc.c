/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:37:06 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/13 19:26:18 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	write_heredoc(int pipefd[2], t_redirect *red)
{
	char	*line;

	close(pipefd[0]);
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, red->file_name) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
	exit(0);
}

void handle_heredoc_input(t_mini *mini, t_redirect *redirect, int write_fd)
{
    (void)mini;     // 未使用であることを明示
    (void)redirect; // 未使用であることを明示

    char *line;

    while (1)
    {
        line = readline("> "); // ユーザー入力を取得
        if (line == NULL) // EOFの場合
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }
        if (strcmp(line, redirect->file_name) == 0) // delimiter と一致
        {
            free(line);
            break;
        }
        write(write_fd, line, strlen(line)); // 入力内容をパイプに書き込み
        write(write_fd, "\n", 1);            // 改行を追加
        free(line);
    }
    close(write_fd); // 書き込みを終了
    exit(0);         // 正常終了
}


int handle_heredoc(t_mini *mini, t_redirect *redirect)
{
    int pipefd[2];
    pid_t pid;
    int status;

    if (pipe(pipefd) == -1)
        system_error(mini);

    pid = fork();
    if (pid == -1)
        system_error(mini);
    else if (pid == 0) // 子プロセス
    {
        setup_heredoc_signal_handlers(); // heredoc中のシグナル設定
        close(pipefd[0]); // 読み取り側を閉じる
        handle_heredoc_input(mini, redirect, pipefd[1]); // 入力処理
    }
    else // 親プロセス
    {
        close(pipefd[1]); // 書き込み側を閉じる
        waitpid(pid, &status, 0); // 子プロセスの終了を待つ

        if (WIFSIGNALED(status)) // シグナルで終了した場合
        {
            if (WTERMSIG(status) == SIGINT) // SIGINT を検出
            {
                mini->status = 130;
                close(pipefd[0]);
                return -1;
            }
            else if (WTERMSIG(status) == SIGQUIT) // SIGQUIT を無視
            {
                mini->status = 131; // 必要に応じてコードを設定
                close(pipefd[0]);
                return -1;
            }
        }
        if (WEXITSTATUS(status) != 0)
        {
            close(pipefd[0]);
            return -1;
        }
    }
    return pipefd[0]; // 読み取り用のファイルディスクリプタを返す
}
