/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <mkaihori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:10:32 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/01 16:56:04 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	print_token(t_token *token)
// {
// 	int	index;

// 	index = 1;
// 	printf("-----debug-----\n");
// 	while (token->type != TOKEN_EOF)
// 	{
// 		printf("token%d\n", index);
// 		printf("token type : %d\n", token->type);
// 		printf("token word : %s\n\n", token->word);
// 		token = token->next;
// 		index++;
// 	}
// 	printf("-----debug-----\n");
// 	return ;
// }

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
		{
			add_history(line);
			tokens = tokenize(line);
			if (tokens)
			{
				ast = parse(&tokens);
				if (ast)
				{
					execute(ast, env_list, &status);
					free_ast(ast);
				}
				free_token_list(tokens);
			}
		}
		free(line);
	}
	free_env_list(env_list);
	return (status);
}
