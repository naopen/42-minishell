/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:10:32 by nkannan           #+#    #+#             */
/*   Updated: 2024/11/16 12:49:36 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int argc, char **argv, char **environ)
{
	(void)argc;
	(void)argv;
	char	*line;
	t_token	*tokens;
	t_node	*ast;
	t_env	*env_list;
	int		status;

	env_list = create_env_list(environ);
	signal(SIGINT, handle_sigint);
	status = EXIT_SUCCESS;
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			break ;
		if (*line != '\0')
			add_history(line);
		tokens = tokenize(line);
		ast = parse(&tokens);
		status = execute(ast, env_list);
		free_token_list(tokens);
		free_ast(ast);
		free(line);
	}
	free_env_list(env_list);
	return (status);

}
