/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:02:03 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 14:02:42 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child_process(t_mini *mini, int pipefd[][2], int process, int p_num)
{
	int	closer;

	setup_child_signal_handlers();
	if (p_num > 0)
	{
		if (dup2(pipefd[p_num - 1][0], STDIN_FILENO) == -1)
			system_error(mini);
	}
	if (p_num < process - 1)
	{
		if (dup2(pipefd[p_num][1], STDOUT_FILENO) == -1)
			system_error(mini);
	}
	closer = 0;
	while (closer < process - 1)
	{
		close(pipefd[closer][0]);
		close(pipefd[closer][1]);
		closer++;
	}
	execute_command(mini, process_command(mini->node, p_num), mini->env,
		&(mini->status));
	exit(mini->status);
}

void	parent_process(t_mini *mini, int pipefd[][2], int process, int pid[])
{
	int	closer;
	int	raw_status;

	closer = 0;
	while (closer < process - 1)
	{
		close(pipefd[closer][0]);
		close(pipefd[closer][1]);
		closer++;
	}
	closer = 0;
	while (closer < process)
	{
		waitpid(pid[closer], &raw_status, 0);
		if (WIFEXITED(raw_status))
			mini->status = WEXITSTATUS(raw_status);
		else if (WIFSIGNALED(raw_status))
			mini->status = 128 + WTERMSIG(raw_status);
		else
			mini->status = raw_status;
		closer++;
	}
	return ;
}

void	prepare_pipe(t_mini *mini, int process, int pipefd[][2])
{
	int	i;

	i = 0;
	while (i < process)
	{
		if (pipe(pipefd[i]) == -1)
			system_error(mini);
		i++;
	}
	return ;
}

void	execute_pipeline(t_mini *mini, t_node *node, int process)
{
	int	p_num;

	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	p_num = 0;
	prepare_pipe(mini, process - 1, mini->pipefd);
	if (node->next == NULL)
		return (execute_command(mini, node, mini->env, &(mini->status)));
	while (p_num < process)
	{
		mini->pid[p_num] = fork();
		if (mini->pid[p_num] == -1)
			system_error(mini);
		if (!mini->pid[p_num])
			child_process(mini, mini->pipefd, process, p_num);
		p_num++;
	}
	parent_process(mini, mini->pipefd, process, mini->pid);
	return ;
}

int	count_node(t_node *node)
{
	int	i;

	i = 0;
	while (node)
	{
		i++;
		node = node->next;
	}
	return (i);
}
