/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:29:30 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/02 16:40:35 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	redirect_in(t_mini *mini, t_redirect *red)
{
	int	fd;

	fd = open(red->file_name, O_RDONLY);
	if (fd == -1)
		system_error(mini);
	if (dup2(fd, STDIN_FILENO) == -1)
		system_error(mini);
	close(fd);
	return ;
}

void	redirect_out(t_mini *mini, t_redirect *red)
{
	int	fd;

	fd = open(red->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		system_error(mini);
	if (dup2(fd, STDOUT_FILENO) == -1)
		system_error(mini);
	close(fd);
	return ;
}

void	redirect_append(t_mini *mini, t_redirect *red)
{
	int	fd;

	fd = open(red->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		system_error(mini);
	if (dup2(fd, STDOUT_FILENO) == -1)
		system_error(mini);
	close(fd);
	return ;
}

int	do_redirection(t_mini *mini, t_redirect *redirect)
{
	while (redirect)
	{
		if (redirect->type == REDIRECT_IN)
			redirect_in(mini, redirect);
		else if (redirect->type == REDIRECT_OUT)
			redirect_out(mini, redirect);
		else if (redirect->type == REDIRECT_APPEND)
			redirect_append(mini, redirect);
		else if (redirect->type == REDIRECT_HEREDOC)
			handle_heredoc(mini, redirect);
		redirect = redirect->next;
	}
	return (0);
}
