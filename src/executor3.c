/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:29:03 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 16:17:27 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_command_not_found(char *argv0, t_mini *mini)
{
	if (ft_strchr(argv0, '/'))
		print_error(mini, "minishell: %s: No such file or directory\n", argv0);
	else
		print_error(mini, "minishell: %s: command not found\n", argv0);
}

void	update_status(t_mini *mini, int *status)
{
	if (WIFEXITED(*status))
	{
		*status = WEXITSTATUS(*status);
		mini->status = *status;
	}
	else if (WIFSIGNALED(*status))
	{
		*status = 128 + WTERMSIG(*status);
		mini->status = *status;
	}
}

t_node	*process_command(t_node *node, int p_num)
{
	int	i;

	i = 0;
	while (i < p_num)
	{
		node = node->next;
		i++;
	}
	return (node);
}
