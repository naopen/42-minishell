/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:06:37 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/04 17:26:56 by mkaihori         ###   ########.fr       */
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
# include <sys/wait.h>

# define SYNTAX_ERROR 1
# define ERROR_OPEN_REDIR 2
# define HEREDOC_TMPFILE "/tmp/minishell_heredoc.tmp"

typedef enum e_token_type
{
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_WORD,
	TOKEN_OPERATOR,
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
	t_redirect_type		type;
	char				*file_name;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_node
{
	t_node_type			type;
	char				**argv;
	struct s_redirect	*redirects;
	struct s_node		*next;
}	t_node;

typedef struct s_env
{
	char				*name;
	char				*value;
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

typedef struct s_mini
{
	struct s_token	*token;
	struct s_node	*node;
	struct s_env	*env;
	char			*line;
	int				(*pipefd)[2];
	int				*pid;
	int				process;
	int				backup_in;
	int				backup_out;
	int				status;
}	t_mini;

// utils.c

void			system_error(t_mini *mini);
bool			ft_isnumber(const char *str);
char			*ft_strndup(t_mini *mini, const char *s, size_t n);
void			ft_strdel(char **as);
void			ft_strarrdel(char **arr);
int				ft_strarrlen(char **arr);
char			**ft_strarradd(t_mini *mini, char **arr, char *str);

// tokenizer.c
t_token			*tokenize(t_mini *mini, char *line);
void			free_token_list(t_token *token_list);
bool			is_quote(char c);

// tokennizer_utils.c
bool			is_metachar(char c);

// parser.c
t_node			*parse(t_mini *mini, t_token **token_list);

// expander.c
void			expand(t_mini *mini, t_node *node, t_env *env_list);
char			*expand_env_var(t_mini *mini, char *str);

// executor.c
void			execute(t_mini *mini);
int				handle_heredoc(t_mini *mini, t_redirect *redirect);

// builtin.c
t_builtin_type	get_builtin_type(const char *cmd);
int				execute_builtin(t_mini *mini, t_builtin_type type, char **argv,
					t_env **env_list);

// env.c
t_env			*create_env_list(t_mini *mini, char **environ);
char			*get_env_value(t_env *env_list, const char *name);
int				set_env_value(t_mini *mini, t_env **env_list,
					const char *name, const char *value);
void			unset_env_value(t_env **env_list, const char *name);
void			print_env_list(t_env *env_list);
void			free_env(t_env *env);
char			**env_to_envp(t_mini *mini, t_env *env_list);

void			syntax_error(t_mini *mini, t_token *token);
void			custom_error(t_mini *mini, char *msg, int error);
int				do_redirection(t_mini *mini, t_redirect *redirect);
int				finish_mini(t_mini *mini);
void			free_node(t_node *node);

#endif
