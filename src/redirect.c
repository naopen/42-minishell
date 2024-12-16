/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:29:30 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/16 14:36:52 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	redirect_in(t_mini *mini, t_redirect *red)
{
	int	fd;

	fd = open(red->file_name, O_RDONLY);
	if (fd == -1)
		return (print_redirect_error(mini, red->file_name));
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (print_redirect_error(mini, red->file_name));
	}
	close(fd);
	return (0);
}

int	redirect_out(t_mini *mini, t_redirect *red)
{
	int	fd;

	fd = open(red->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (print_redirect_error(mini, red->file_name));
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (print_redirect_error(mini, red->file_name));
	}
	close(fd);
	return (0);
}

int	redirect_append(t_mini *mini, t_redirect *red)
{
	int	fd;

	fd = open(red->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (print_redirect_error(mini, red->file_name));
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (print_redirect_error(mini, red->file_name));
	}
	close(fd);
	return (0);
}

static int	do_single_redirection(t_mini *mini, t_redirect *redirect)
{
	int	ret;

	ret = 0;
	if (redirect->type == REDIRECT_IN)
		ret = redirect_in(mini, redirect);
	else if (redirect->type == REDIRECT_OUT)
		ret = redirect_out(mini, redirect);
	else if (redirect->type == REDIRECT_APPEND)
		ret = redirect_append(mini, redirect);
	else if (redirect->type == REDIRECT_HEREDOC)
		ret = handle_heredoc(mini, redirect);
	return (ret);
}

int	do_redirection(t_mini *mini, t_redirect *redirect)
{
	int			ret;
	t_redirect	*current;

	current = redirect;
	while (current)
	{
		ret = do_single_redirection(mini, current);
		if (ret != 0)
			return (ret);
		current = current->next;
	}
	return (0);
}
