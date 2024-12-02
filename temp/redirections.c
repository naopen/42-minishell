/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:09:32 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/02 11:56:40 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_redirections(t_command *cmd)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir != NULL)
	{
		if (do_redirection(redir) == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}

int	do_redirection(t_redirect *redir)
{
	int	fd;

	if (redir->type == REDIR_OUTPUT)
		fd = open_redirection_file(redir->filename,
				O_WRONLY | O_CREAT | O_TRUNC);
	else if (redir->type == REDIR_APPEND)
		fd = open_redirection_file(redir->filename,
				O_WRONLY | O_CREAT | O_APPEND);
	else if (redir->type == REDIR_INPUT)
		fd = open_redirection_file(redir->filename, O_RDONLY);
	else if (redir->type == REDIR_HEREDOC)
		fd = heredoc(redir->filename);
	else
		return (0);
	if (fd == -1)
		return (-1);
	if (dup2(fd, redir->fd) == -1)
		exit_with_error();
	if (close(fd) == -1)
		exit_with_error();
	return (0);
}

int	open_redirection_file(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		perror("minishell: open");
		return (-1);
	}
	return (fd);
}

int	heredoc(char *delimiter)
{
	char	*line;
	int		pfd[2];

	if (pipe(pfd) == -1)
		exit_with_error();
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, pfd[1]);
		free(line);
	}
	if (close(pfd[1]) == -1)
		exit_with_error();
	return (pfd[0]);
}
