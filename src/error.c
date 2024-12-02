#include "../include/minishell.h"

void	system_error(t_mini *mini)
{
	finish_mini(mini);
	perror(NULL);
	exit(errno);
}

void	syntax_error(t_mini *mini, t_token *token)
{
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == - 1)
		system_error(mini);
	if (token->type == TOKEN_EOF)
		printf("syntax error near unexpected token `newline'\n");
	else
		printf("syntax error near unexpected token `%c'\n", token->word[0]);
	if (dup2(mini->backup_out, STDOUT_FILENO) == -1)
		system_error(mini);
	finish_mini(mini);
	exit(258) ;
}

void	custum_error(t_mini *mini, char *msg, int error)
{
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
		system_error(mini);
	printf("%s\n", msg);
	if (dup2(mini->backup_out, STDOUT_FILENO) == -1)
		system_error(mini);
	finish_mini(mini);
	exit(error) ;
}
