/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:34:38 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/09 21:13:03 by shunwata         ###   ########.fr       */
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

void	tokenize(t_alloc *alloc, char *line);
void	free_tokens(t_token *tokens);

void	parse(t_alloc *heap);
t_cmd	*exec_cmd_constructor(void);
t_cmd	*pipe_cmd_constructor(t_cmd *left, t_cmd *right);
t_cmd	*redir_cmd_constructor(t_cmd *subcmd, char *file, int mode, int fd);
void	free_ast(t_cmd *cmd);

void	execute(t_cmd *ast, t_alloc *heap, char **ev);
char	*get_fullpath(char *cmd_name, char **envp, t_alloc *heap);

void	free_2d_array(char **array);

char	*ft_strndup(const char *s1, size_t n);
void	cleanup(t_alloc *alloc);

// for debug.c
// void	print_ast(t_cmd *cmd, int level);

// for builtin_test.c
int		c_echo(char **argv);
int		c_cd(char **argv);
int		c_pwd(char **argv);
int		c_export(char **argv);
int		c_unset(char **argv);
int		c_env(char **argv);
int		c_exit(char **argv);

#endif
