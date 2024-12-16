/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:08:33 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 16:16:48 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/stat.h>

static void	child_process(char **argv, char **envp, char *exec_path,
		t_mini *mini)
{
	struct stat	st;

	if (execve(exec_path, argv, envp) == -1)
	{
		if (errno == ENOENT)
		{
			if (stat(exec_path, &st) == 0 && S_ISDIR(st.st_mode))
			{
				print_error(mini, "minishell: %s: Is a directory\n", argv[0]);
				exit(126);
			}
			handle_command_not_found(argv[0], mini);
			exit(127);
		}
		else if (errno == EACCES)
		{
			print_error(mini, "minishell: %s: Permission denied\n", argv[0]);
			exit(126);
		}
		perror("minishell");
		exit(126);
	}
}

void	execute_external(t_mini *mini, char **argv, t_env *env_list,
		int *status)
{
	pid_t	pid;
	char	**envp;
	char	*exec_path;

	envp = NULL;
	exec_path = find_executable(argv[0], env_list);
	if (!exec_path)
	{
		print_error(mini, "minishell: %s: command not found\n", argv[0]);
		*status = 127;
		return ;
	}
	pid = fork();
	if (pid == -1)
		system_error(mini);
	if (!pid)
	{
		envp = env_to_envp(mini, env_list);
		child_process(argv, envp, exec_path, mini);
	}
	waitpid(pid, status, 0);
	update_status(mini, status);
	free(exec_path);
	return ;
}

void	execute_command(t_mini *mini, t_node *node, t_env *env_list,
		int *status)
{
	t_builtin_type	builtin_type;
	int				i;

	if (node == NULL)
		return ;
	i = 0;
	if (do_redirection(mini, node->redirects) != 0)
	{
		*status = mini->status;
		return ;
	}
	while (node->argv[i] && node->argv[i][0] == '\0')
		i++;
	builtin_type = get_builtin_type(node->argv[i]);
	if (builtin_type != BUILTIN_UNKNOWN)
		mini->status = execute_builtin(mini, builtin_type, node->argv + i,
				&env_list);
	else if (node->argv[i])
		execute_external(mini, node->argv + i, env_list, status);
	else
	{
		print_error(mini, "bash: : command not found\n", NULL);
		mini->status = 127;
	}
}

void	execute(t_mini *mini)
{
	mini->process = count_node(mini->node);
	if (mini->process > 1)
	{
		mini->pipefd = malloc(sizeof(int [2]) * (mini->process - 1));
		if (!mini->pipefd)
			system_error(mini);
	}
	mini->pid = malloc(sizeof(pid_t) * mini->process);
	if (!mini->pid)
		system_error(mini);
	execute_pipeline(mini, mini->node, mini->process);
	if (dup2(mini->backup_in, STDIN_FILENO) == -1)
		system_error(mini);
	if (dup2(mini->backup_out, STDOUT_FILENO) == -1)
		system_error(mini);
	return ;
}
