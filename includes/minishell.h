/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:34:38 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:01:56 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "libft.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum e_node_type
{
	NODE_EXEC,
	NODE_REDIR,
	NODE_PIPE
}	t_node_type;

typedef struct s_cmd
{
	t_node_type		type;
	char			**argv;
	struct s_cmd	*left;
	struct s_cmd	*right;
	struct s_cmd	*subcmd;
	char			*file;
	int				mode;
	int				fd;
}	t_cmd;

typedef struct s_mshell
{
	char	*line;
	t_token	*head;
	t_cmd	*node;
	t_list	*tmp_files;
	char	**ev_clone;
	int		exit_status;
	bool	success;
	int		ac;
	char	**av;
}	t_mshell;

typedef struct s_lexer
{
	const char	*line;
	int			index;
	t_token		*head;
	t_token		*tail;
	t_mshell	*data;
}	t_lexer;

//tokenizer
void	tokenize(t_mshell *data);
void	free_tokens(t_token *tokens);
bool	is_metachar(char c);
void	skip_spaces(t_lexer *lx);
bool	append_input(t_mshell *data);
void	request_missing_quote(t_lexer *lx);

//parser
void	parse(t_mshell *data);
t_cmd	*exec_cmd_constructor(void);
t_cmd	*pipe_cmd_constructor(t_cmd *left, t_cmd *right);
t_cmd	*redir_cmd_constructor(t_cmd *subcmd, char *file, int mode, int fd);
void	free_ast(t_cmd *cmd);
bool	is_redirection(t_token_type type);
bool	is_empty_cmd(t_cmd *cmd);
bool	is_end_cmd(t_token *tokens);

t_cmd	*parse_redirection(t_cmd *cmd, t_token **tokens, t_mshell *data);

// expander
void	expand(t_cmd *node, t_mshell *data);
void	expand_envs(char **str, t_mshell *data);
char	*remove_quotes(const char *str);

//executor
void	execute(t_cmd *node, t_mshell *data);
char	*get_fullpath(char *cmd_name, t_mshell *data);
void	get_exit_status(t_mshell *data, int status);
void	set_pipeend(int pipefd[2], int dest_fd, t_mshell *data);
pid_t	exec_subnode(t_cmd *node, int pipefd[2], int dest_fd, t_mshell *data);
char	**split_path_keep_empty(const char *s);
t_cmd	*get_exec_node(t_cmd *node);
void	backup_stdio(int backups[2], t_mshell *data);
void	restore_stdio(int backups[2], t_mshell *data);
bool	apply_redirections(t_cmd *node);

bool	is_parent_builtin(t_cmd *node);
void	execute_parent_builtin(t_cmd *node, t_mshell *data);
bool	execute_builtin(t_cmd *exec_node, t_mshell *data);

void	heredoc(t_cmd *node, t_mshell *data);
void	cleanup_tmp_files(t_list **list);
bool	is_delimiter(const char *line, const char *delimiter);
char	*generate_tmp_filename(t_mshell *data);

//error
void	puterr(char *cmd_name, char *msg);

//utils
void	cleanup(t_mshell *data);
void	get_input(char **line, const char *message);
void	print_exit(t_mshell *data);

//initialize
void	clone_ev(char **ev, t_mshell *data);

#endif
