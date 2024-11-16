/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:06:37 by nkannan           #+#    #+#             */
/*   Updated: 2024/11/16 17:35:50 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>

# define SYNTAX_ERROR 1
# define ERROR_OPEN_REDIR 2
# define HEREDOC_TMPFILE "/tmp/minishell_heredoc.tmp"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OPERATOR,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*word;
	struct s_token	*next;
}	t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
}	t_node_type;

typedef enum e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC,
}	t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type	type;
	char			*file_name;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_node
{
	t_node_type		type;
	char			**argv;
	struct s_redirect	*redirects;
	struct s_node		*next;
}	t_node;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env		*next;
}	t_env;

typedef enum e_builtin_type
{
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT,
	BUILTIN_UNKNOWN,
}	t_builtin_type;

/*
** utils.c
*/
void	exit_with_error(const char *msg);
bool	ft_isnumber(const char *str);
char	*ft_strndup(const char *s1, size_t n);
void	ft_strdel(char **as);
void	ft_strarrdel(char **arr);
int		ft_strarrlen(char **arr);
char	**ft_strarradd(char **arr, char *str);

/*
** tokenizer.c
*/
t_token	*tokenize(char *line);
void	free_token_list(t_token *token_list);
bool	is_quote(char c);

/*
** parser.c
*/
t_node	*parse(t_token **token_list);
void	free_ast(t_node *ast);

/*
** expander.c
*/
void	expand(t_node *node, t_env *env_list);

/*
** executor.c
*/
int		execute(t_node *node, t_env *env_list);
int		handle_heredoc(t_redirect *redirect);

/*
** builtin.c
*/
t_builtin_type	get_builtin_type(const char *cmd);
int				execute_builtin(t_builtin_type type, char **argv,
					t_env **env_list);

/*
** env.c
*/
t_env	*create_env_list(char **environ);
char	*get_env_value(t_env *env_list, const char *name);
int		set_env_value(t_env **env_list, const char *name,
			const char *value);
void	unset_env_value(t_env **env_list, const char *name);
void	print_env_list(t_env *env_list);
void	free_env_list(t_env *env_list);
char	**env_to_envp(t_env *env_list);

#endif
