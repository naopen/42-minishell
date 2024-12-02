/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:10:32 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/02 12:11:09 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_token(t_token *token)
{
	int	index;

	index = 1;
	printf("-----debug-----\n");
	while (token->type != TOKEN_EOF)
	{
		printf("token%d\n", index);
		printf("token type : %d\n", token->type);
		printf("token word : %s\n\n", token->word);
		token = token->next;
		index++;
	}
	printf("-----debug-----\n");
	return ;
}

void	print_node(t_node *node)
{
	int			i;
	t_redirect	*temp;

	i = 0;
	printf("=====node debug=====\n");
	while (node)
	{
		printf("node type : %d\n", node->type);
		while (node->argv[i])
		{
			printf("node argv %d: %s\n", i + 1, node->argv[i]);
			i++;
		}
		i = 0;
		temp = node->redirects;
		while (temp)
		{
			printf("redirects type : %d\n", temp->type);
			printf("redirects filename : %s\n", temp->file_name);
			temp = temp->next;
		}
		node = node->next;
		printf("\n");
	}
	printf("=====node debug=====\n");
	return ;
}

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
	char	*line;
	t_token	*tokens;
	t_node	*ast;
	t_env	*env_list;
	int		status;

	(void)argc;
	(void)argv;
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
			free(line);
			if (tokens)
			{
				ast = parse(&tokens);
				free_token_list(tokens);
				if (ast)
				{
					execute(ast, env_list, &status);
					free_ast(ast);
				}
			}
		}
	}
	free_env_list(env_list);
	return (status);
}
