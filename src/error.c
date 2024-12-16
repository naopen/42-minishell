/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:43:04 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 16:09:55 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	system_error(t_mini *mini)
{
	finish_mini(mini);
	perror(NULL);
	exit(errno);
}

void	syntax_error(t_mini *mini, t_token *token)
{
	char	*unexpected;

	if (token->type == TOKEN_EOF)
		unexpected = "newline";
	else
		unexpected = token->word;
	print_error(mini, "bash: line 1: syntax error near unexpected token `%s'\n",
		unexpected);
	print_error(mini, "bash: line 1: `%s'\n", mini->line);
	mini->status = 2;
	return ;
}

void	print_error(t_mini *mini, char *msg, char *arg)
{
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
		system_error(mini);
	if (arg)
		printf(msg, arg);
	else
		printf("%s", msg);
	if (dup2(mini->backup_out, STDOUT_FILENO) == -1)
		system_error(mini);
	return ;
}

void	custom_error(t_mini *mini, char *msg, int error)
{
	print_error(mini, msg, NULL);
	finish_mini(mini);
	exit(error);
}

int	print_redirect_error(t_mini *mini, char *filename)
{
	print_error(mini, "minishell: %s: ", filename);
	print_error(mini, "%s\n", strerror(errno));

	mini->status = 1;
	return (1);
}
