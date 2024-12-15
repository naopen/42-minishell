/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:10:32 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/15 19:12:41 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t g_cntl_c = 0;

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

void	print_env(t_env *env)
{
	while (env)
	{
		printf("name : %s\n", env->name);
		printf("value : %s\n", env->value);
		env = env->next;
	}
	return ;
}

void	free_mini(t_mini *mini)
{
	free_token_list(mini->token);
	mini->token = NULL;
	free_node(mini->node);
	mini->node = NULL;
	if (mini->line)
		free(mini->line);
	mini->line = NULL;
	if (mini->pipefd)
		free(mini->pipefd);
	mini->pipefd = NULL;
	if (mini->pid)
		free(mini->pid);
	mini->pid = NULL;
	mini->process = -1;
	return ;
}

int	finish_mini(t_mini *mini)
{
	int	status;

	free_mini(mini);
	if (mini->backup_in != -1)
		close(mini->backup_in);
	mini->backup_in = -1;
	if (mini->backup_out != -1)
		close(mini->backup_out);
	mini->backup_out = -1;
	free_env(mini->env);
	mini->env = NULL;
	status = mini->status;
	free(mini);
	return (status);
}

t_mini	*get_mini(char **environ)
{
	t_mini	*mini;

	mini = (t_mini *)malloc(sizeof(t_mini) * 1);
	if (!mini)
		system_error(mini);
	mini->token = NULL;
	mini->node = NULL;
	mini->line = NULL;
	mini->env = create_env_list(mini, environ);
	mini->status = EXIT_SUCCESS;
	mini->pipefd = NULL;
	mini->pid = NULL;
	mini->process = -1;
	mini->backup_in = -1;
	mini->backup_out = -1;
	mini->backup_in = dup(STDIN_FILENO);
	if (mini->backup_in == -1)
		system_error(mini);
	mini->backup_out = dup(STDOUT_FILENO);
	if (mini->backup_out == -1)
		system_error(mini);
	return (mini);
}

// void	minishell(t_mini *mini)
// {
// 	while (1)
// 	{
// 		mini->line = readline("minishell> ");
// 		if (mini->line == NULL)
// 			break ;
// 		if (*(mini->line) != '\0')
// 		{
// 			add_history(mini->line);
// 			mini->token = tokenize(mini, mini->line);
// 			mini->node = parse(mini, &(mini->token));
// 			execute(mini);
// 			free_mini(mini);
// 		}
// 	}
// 	exit (finish_mini(mini));
// }

int	main(int argc, char **argv, char **environ)
{
	t_mini	*mini;

	(void)argc;
	(void)argv;
	mini = get_mini(environ);
	setup_signal_handlers();
	while (1)
	{
		mini->line = readline("> ");
		if (mini->line == NULL)
			break ;
		if (*(mini->line) != '\0')
		{
			if (g_cntl_c)
				mini->status = g_cntl_c;
			add_history(mini->line);
			mini->token = tokenize(mini, mini->line);
			mini->node = parse(mini, &(mini->token));
			execute(mini);
			free_mini(mini);
		}
		g_cntl_c = 0;
	}
	return (finish_mini(mini));
}
