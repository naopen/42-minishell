/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:06:37 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 15:19:22 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# ifdef __linux__
#  include <linux/limits.h>
# endif

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
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*word;
	struct s_token				*next;
}								t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
}								t_node_type;

typedef enum e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC,
}								t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type				type;
	char						*file_name;
	struct s_redirect			*next;
}								t_redirect;

typedef struct s_node
{
	t_node_type					type;
	char						**argv;
	struct s_redirect			*redirects;
	struct s_node				*next;
}								t_node;

typedef struct s_env
{
	char						*name;
	char						*value;
	struct s_env				*next;
}								t_env;

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
}								t_builtin_type;

typedef struct s_mini
{
	struct s_token				*token;
	struct s_node				*node;
	struct s_env				*env;
	char						*line;
	int							(*pipefd)[2];
	int							*pid;
	int							process;
	int							backup_in;
	int							backup_out;
	int							status;
}								t_mini;

extern volatile sig_atomic_t	g_cntl_c;

// utils.c
void							system_error(t_mini *mini);
bool							ft_isnumber(const char *str);
char							*ft_strndup(t_mini *mini, const char *s,
									size_t n);
void							ft_strdel(char **as);
void							ft_strarrdel(char **arr);
int								ft_strarrlen(char **arr);
char							**ft_strarradd(t_mini *mini, char **arr,
									char *str);

// builtin.c
t_builtin_type					get_builtin_type(const char *cmd);
int								execute_builtin(t_mini *mini,
									t_builtin_type type, char **argv,
									t_env **env_list);

// builtin2.c
int								builtin_unset(char **argv, t_env **env_list);
int								builtin_env(char **argv, t_env **env_list);
int								builtin_pwd(char **argv);
int								builtin_exit(t_mini *mini, char **argv);



// builtin3.c
bool							is_valid_identifier(t_mini *mini,
									const char *str);
int								builtin_export(t_mini *mini, char **argv,
									t_env **env_list);


// env.c
t_env							*create_env_list(t_mini *mini, char **environ);
char							*get_env_value(t_env *env_list,
									const char *name);
int								set_env_value(t_mini *mini, const char *name,
									const char *value);
void							unset_env_value(t_env **env_list,
									const char *name);
void							print_env_list(t_env *env_list, int flag);
void							free_env(t_env *env);
char							**env_to_envp(t_mini *mini, t_env *env_list);

void							syntax_error(t_mini *mini, t_token *token);
void							custom_error(t_mini *mini, char *msg,
									int error);
int								do_redirection(t_mini *mini,
									t_redirect *redirect);
int								finish_mini(t_mini *mini);
void							free_node(t_node *node);
void							print_error(t_mini *mini, char *msg, char *arg);

// env2.c
char							*get_env(t_mini *mini, char *env_name);
void							unset_env_value(t_env **env_list, const char *name);
void							print_env_list(t_env *env_list, int flag);

// env3.c
void							add_env_list(t_env **head, t_env *new);
t_env							*create_env_list(t_mini *mini, char **environ);

// error.c
int								print_redirect_error(t_mini *mini, char *filename);

// executor.c
void							execute(t_mini *mini);
int								handle_heredoc(t_mini *mini,
									t_redirect *redirect);
t_node							*process_command(t_node *node, int p_num);
void							execute_command(t_mini *mini, t_node *node, t_env *env_list, int *status);


// executor2.c
void							execute_pipeline(t_mini *mini, t_node *node,
									int process);
int								count_node(t_node *node);

// expander.c
void							expand(t_mini *mini, t_node *node,
									t_env *env_list);
char							*expand_env_var(t_mini *mini, char *str);

// parser.c
t_node							*new_node(t_mini *mini, t_node_type type);
char							*get_filename(t_mini *mini, t_token *token);
void							add_redirect(t_mini *mini, t_node *node,
									t_token **token_list);
t_node							*parse_process(t_mini *mini,
									t_token **token_list);
t_node							*parse(t_mini *mini, t_token **token_list);

// parser2.c
char							**get_command_args(t_mini *mini,
									t_token **token_list, int argc);
void							add_redirect_to_list(t_redirect **head,
									t_redirect *red);
char							*cut_quote(t_mini *mini, char *word,
									int single_q, int double_q);

// signal.c
void							handle_sigint(int sig);
void							setup_signal_handlers(void);
void							setup_child_signal_handlers(void);
void							handle_heredoc_sigint(int sig);
void							setup_heredoc_signal_handlers(void);

// tokenizer.c
t_token_type					get_token_type(const char *str);
t_token							*new_token(t_mini *mini, t_token_type type,
									char *word);
t_token							*split_token(t_mini *mini, char **line);
t_token							*tokenize(t_mini *mini, char *line);
void							free_token_list(t_token *token_list);
bool							is_quote(char c);

// tokenizer2.c

int								get_opsize(t_mini *mini, char **line);
t_token							*split_token_op(t_mini *mini, char **line);
void							add_token_to_list(t_token **head,
									t_token *token);

// tokennizer_utils.c
bool							is_metachar(char c);
bool							is_redirect(char c);
void							in_quote(t_mini *mini, char **line);
char							*get_env(t_mini *mini, char *env_name);

#endif
