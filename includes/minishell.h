/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:34:38 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/04 01:47:41 by nmasuda          ###   ########.fr       */
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

# include "builtin.h"
# include "minishell_signal.h"

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

// expander
void	expand(t_cmd *ast, t_alloc *heap);

//executor
void	execute(t_cmd *ast, t_alloc *heap);
char	*get_fullpath(char *cmd_name, t_alloc *heap);
char	**split_path_keep_empty(const char *s);
void	get_exit_status(t_alloc *heap, int status);

bool	is_parent_builtin(t_cmd *ast);
bool	execute_builtin(t_cmd *exec_node, t_alloc *heap);

void	cleanup_temp_files(t_list **list);
void	find_and_process_heredocs(t_cmd *ast, t_alloc *heap);

//error
void	puterr(char *cmd_name, char *msg);

//utils
void	cleanup(t_alloc *alloc);
void	get_input(char **line, const char *message);
void	print_exit(t_alloc *heap);
void	free_2d_array(char ***array);

//initialize
void	clone_ev(char **ev, t_alloc *heap);

#endif
