/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:37:06 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/02 16:40:31 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	wirte_heredoc(int pipefd[2], t_redirect *red)
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

int	handle_heredoc(t_mini *mini, t_redirect *redirect)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		system_error(mini);
	pid = fork();
	if (pid == -1)
		system_error(mini);
	if (pid == 0)
		wirte_heredoc(pipefd, redirect);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	exit(0);
}
