/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 18:43:41 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/15 21:43:35 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_cntl_c = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_handlers(void)
{
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
}

void	setup_child_signal_handlers(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
}

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(1);
}

void	setup_heredoc_signal_handlers(void)
{
	if (signal(SIGINT, handle_heredoc_sigint) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
}
