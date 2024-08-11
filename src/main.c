/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:10:32 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/11 15:25:54 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **environ)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmd;
	t_env		*env_head;
	int			exit_code; // 変数名変更

	(void)argc;
	(void)argv;
	env_head = create_env_list(environ);
	setup_signal_handlers();
	exit_code = 0; // 変数名変更
	while (1)
	{
		line = readline(PROMPT);
		if (line == NULL)
			break ;
		if (*line != '\0')
		{
			add_history(line);
			tokens = tokenize(line);
			cmd = parse(tokens);
			if (cmd == NULL)
			{
				printf("minishell: syntax error\n");
				exit_code = 258; // 変数名変更
			}
			else
			{
				expand_tokens(tokens, exit_code); // 変数名変更
				exit_code = execute_pipeline(cmd, &env_head); // 変数名変更
			}
			free_tokens(tokens);
			free_commands(cmd);
		}
		free(line);
	}
	free_env_list(env_head);
	return (exit_code); // 変数名変更
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->str);
		free(tokens);
		tokens = tmp;
	}
}

void	free_redirects(t_redirect *redirects)
{
	t_redirect	*tmp;

	while (redirects)
	{
		tmp = redirects->next;
		free(redirects->filename);
		free(redirects);
		redirects = tmp;
	}
}

void	free_commands(t_command *cmds)
{
	t_command	*tmp;
	int			i;

	while (cmds)
	{
		tmp = cmds->next;
		i = 0;
		while (cmds->argv[i])
		{
			free(cmds->argv[i]);
			i++;
		}
		free(cmds->argv);
		free_redirects(cmds->redirects);
		free(cmds);
		cmds = tmp;
	}
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
