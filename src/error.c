#include "../include/minishell.h"

void	system_error(t_mini *mini)
{
	finish_mini(mini);
	perror(NULL);
	exit(errno);
}

void	syntax_error(t_mini *mini, t_token *token)
{
	char *unexpected;

	if (token->type == TOKEN_EOF)
		unexpected = "newline";
	else
		unexpected = token->word;

	fprintf(stderr, "bash: line 1: syntax error near unexpected token `%s'\n", unexpected);
	fprintf(stderr, "bash: line 1: `%s'\n", mini->line);

	mini->status = 2;
	finish_mini(mini);
	exit(2);
}

void	custom_error(t_mini *mini, char *msg, int error)
{
	if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
		system_error(mini);
	printf("%s\n", msg);
	if (dup2(mini->backup_out, STDOUT_FILENO) == -1)
		system_error(mini);
	finish_mini(mini);
	exit(error);
}
