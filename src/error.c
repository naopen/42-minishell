#include "../include/minishell.h"

void	exit_with_error(void)
{
	perror(NULL);
	exit(errno);
}

void	syntax_error(t_token *token)
{
	int	backup;

	backup = dup(STDOUT_FILENO);
	if (backup == -1)
		exit_with_error();
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
		exit_with_error();
	if (token->type == TOKEN_EOF)
		printf("syntax error near unexpected token `newline'\n");
	else
		printf("syntax error near unexpected token `%c'\n", token->word[0]);
	if (dup2(backup, STDOUT_FILENO) == -1)
		exit_with_error();
	exit(258) ;
}

void	custum_error(char *msg, int error)
{
	int	backup;

	backup = dup(STDOUT_FILENO);
	if (backup == -1)
		exit_with_error();
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
		exit_with_error();
	printf("%s\n", msg);
	if (dup2(backup, STDOUT_FILENO) == -1)
		exit_with_error();
	exit(error);
}
