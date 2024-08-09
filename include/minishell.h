/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:06:37 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/08 18:31:29 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include <errno.h>
# include <signal.h>
# include <limits.h>
# include <stdbool.h>

# define PROMPT "minishell$ "
# define REDIR_OUTPUT 1
# define REDIR_APPEND 2
# define REDIR_INPUT 3
# define REDIR_HEREDOC 4
# define ERROR_TOKENIZE 1
# define ERROR_OPEN_REDIR 2
# define FNM_NOMATCH 1

int	g_last_exit_status = 0;

typedef enum e_token_kind
{
	TK_WORD,
	TK_REDIR_OUTPUT,
	TK_REDIR_APPEND,
	TK_REDIR_INPUT,
	TK_REDIR_HEREDOC,
	TK_PIPE,
	TK_EOF,
}	t_token_kind;

typedef struct s_token
{
	t_token_kind	kind;
	char			*str;
	struct s_token	*next;
}	t_token;

typedef struct s_redirect
{
	int				type;
	int				fd;
	char			*filename;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char			**argv;
	t_redirect		*redirects;
	struct s_command	*next;
}	t_command;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// utils.c
void	fatal_error(const char *msg);
size_t		ft_strlen(const char *s);
char	*ft_strndup(const char *s, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_char_free(char *s1, char s2);
char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_strjoin_space_free(char *s1, char *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strtok(char *str, const char *delim);
int		ft_isalnum(int c);
int		ft_atoi(const char *str);
char	*ft_itoa(int n);
int		ft_fnmatch(const char *pattern, const char *string, int flags);
char	*ft_strchr(const char *s, int c);
void	ft_putendl_fd(char *s, int fd);


// lexer.c
t_token	*tokenize(char *line);
t_token	*new_token(t_token_kind kind, char *str);

// parser.c
t_command	*parse(t_token *tokens);
t_command	*new_command(void);
t_redirect	*new_redirect(t_token *tokens);
void		append_tok(t_token **tokens, t_token *tok);
void		append_redirect(t_redirect **redirects, t_redirect *redir);

// executor.c
int			execute_pipeline(t_command *cmd, t_env **env_head);
pid_t		execute_command(t_command *cmd, int pipefd[2],
				int prev_pipefd[2], t_env **env_head);
char		*find_command_path(char *command);
void		close_pipe(int pipefd[2]);
void		prepare_pipe(t_command *cmd, int pipefd[2], int prev_pipefd[2]);
int			is_builtin(char *command);
int			execute_builtin(char *command, char **argv, t_env **env_head);

// builtin.c
int			builtin_echo(char **argv);
int			builtin_cd(char **argv);
int			builtin_pwd(char **argv);
int			builtin_exit(char **argv);
int			builtin_export(char **argv, t_env **env_head);
int			builtin_unset(char **argv, t_env **env_head);
int			builtin_env(char **argv, t_env *env);

// env.c
char	**create_environ(t_env *env_head);
t_env		*create_env_list(char **environ);
void		print_env(t_env *env);
t_env		*search_env(t_env *env, char *key);
void	append_env(t_env **head, t_env *env);

// redirections.c
int			handle_redirections(t_command *cmd);
int			do_redirection(t_redirect *redir);
int			open_redirection_file(char *filename, int flags);
int			heredoc(char *delimiter);

// expand.c
void		expand_tokens(t_token *tokens, int last_status);
char		*expand_variable(char *str, int last_status);
char		*expand_special_parameter(char *str, int last_status);
void		remove_quotes(char *str);
char		*expand_wildcard(char *str);

// signal.c
void		setup_signal_handlers(void);
void		handle_sigint(int sig);

// main.c
void	free_tokens(t_token *tokens);
void	free_commands(t_command *cmds);
void	free_env_list(t_env *env);


#endif
