#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_split_err
{
	NO_ERR,
	SYNTAX_ERR
}	t_split_err;

typedef enum e_get_fpath_err
{
	NO_ERR,
	MALLOC_FAILED,
	NO_SUCH_FORD,
	PERM_DENIED
}	t_get_fpath_err;

#endif
