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

void	print_error(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	return ;
}

void	custom_error(t_mini *mini, char *msg, int error)
{
	print_error(msg);
	finish_mini(mini);
	exit(error);
}
