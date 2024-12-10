/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:29:30 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/10 21:28:10 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int print_redirect_error(t_mini *mini, char *filename)
{
    fprintf(stderr, "minishell: %s: %s\n", filename, strerror(errno));
    mini->status = 1;
	return (1);
}

int	redirect_in(t_mini *mini, t_redirect *red)
{
	int	fd;

	fd = open(red->file_name, O_RDONLY);
	if (fd == -1)
		return print_redirect_error(mini, red->file_name);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return print_redirect_error(mini, red->file_name);
	}
	close(fd);
	return (0);
}

int	redirect_out(t_mini *mini, t_redirect *red)
{
	int	fd;

	fd = open(red->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return print_redirect_error(mini, red->file_name);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return print_redirect_error(mini, red->file_name);
	}
	close(fd);
	return (0);
}

int	redirect_append(t_mini *mini, t_redirect *red)
{
	int	fd;

	fd = open(red->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return print_redirect_error(mini, red->file_name);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return print_redirect_error(mini, red->file_name);
	}
	close(fd);
	return (0);
}

int	do_redirection(t_mini *mini, t_redirect *redirect)
{
	int	ret;

	while (redirect)
	{
		if (redirect->type == REDIRECT_IN)
		{
			ret = redirect_in(mini, redirect);
			if (ret != 0)
				return ret;
		}
		else if (redirect->type == REDIRECT_OUT)
		{
			ret = redirect_out(mini, redirect);
			if (ret != 0)
				return ret;
		}
		else if (redirect->type == REDIRECT_APPEND)
		{
			ret = redirect_append(mini, redirect);
			if (ret != 0)
				return ret;
		}
		else if (redirect->type == REDIRECT_HEREDOC)
		{
			ret = handle_heredoc(mini, redirect);
			if (ret != 0)
				return ret;
		}
		redirect = redirect->next;
	}
	return (0);
}
