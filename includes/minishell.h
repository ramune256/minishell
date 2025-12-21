/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:34:38 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/21 20:47:15 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>

# include "libft.h"

/* --- トークンのデータ構造 --- */
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

/* --- 抽象構文木(AST)のデータ構造 --- */
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

typedef struct s_alloc
{
	char	*line;
	t_token	*head;
	t_cmd	*ast;
	t_list	*temp_files;
	char	**ev_clone;
	int		exit_status;
	bool	success;
}	t_alloc;

// typedef enum e_split_err
// {
// 	NO_ERR,
// 	SYNTAX_ERR
// }	t_split_err;

// typedef enum e_error
// {
// 	NO_ERR,
// 	MALLOC_FAILED,
// 	NO_SUCH_FORD,
// 	PERM_DENIED
// }	t_error;

//tokenizer
void	tokenize(t_alloc *alloc);
void	free_tokens(t_token *tokens);
bool	is_metachar(char c);

//parser
void	parse(t_alloc *heap);
t_cmd	*exec_cmd_constructor(void);
t_cmd	*pipe_cmd_constructor(t_cmd *left, t_cmd *right);
t_cmd	*redir_cmd_constructor(t_cmd *subcmd, char *file, int mode, int fd);
void	free_ast(t_cmd *cmd);

bool	is_redirection(t_token_type type);
t_cmd	*parse_redirection(t_cmd *cmd, t_token **tokens, t_alloc *heap);

//expander
void	expander(t_cmd *ast, t_alloc *heap);
char	*chenge_ev(char *line, t_alloc *heap, size_t *j, size_t *ev_len);
void	flag_quote(size_t *j, int *s_flag, int *d_flag, char *cur);
char	*count_quote(char *cur);
void	be_zero(int *s_flag, int *d_flag, size_t *j);
bool	input_argv(int *i, char *cur, t_cmd *ast);
char	*update_cur(size_t j, char *tmp, char *tail, char *cur);

//executor
void	execute(t_cmd *ast, t_alloc *heap);
char	*get_fullpath(char *cmd_name, t_alloc *heap);
void	change_fd(int pipefd[2], int target_fd, int fd_num);

bool	is_parent_builtin(t_cmd *ast);
bool	execute_builtin(t_cmd *exec_node, t_alloc *heap);

void	cleanup_temp_files(t_list **list);
void	find_and_process_heredocs(t_cmd *ast, t_alloc *heap);

//utils
char	*ft_strndup(const char *s1, size_t n);
int		ft_strcmp(const char *s1, const char *s2);
void	cleanup(t_alloc *alloc);
void	get_input(char **line, const char *message);
void	print_exit(void);
void	free_2d_array(char ***array);

//initialize
void	clone_ev(char **ev, t_alloc *heap);

// for debug.c
// void	print_ast(t_cmd *cmd, int level);

// for builtin_test.c
// int		c_echo(char **argv);
// int		c_cd(char **argv);
// int		c_pwd(char **argv);
// int		c_export(char **argv);
// int		c_unset(char **argv);
// int		c_env(char **argv);
// int		c_exit(char **argv);

#endif
