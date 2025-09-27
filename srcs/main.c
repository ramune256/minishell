#include "minishell.h"

static int	exec_command(char *input, char **envp)
{
	pid_t			pid;
	char			**cmd_args;
	char			*cmd_fullpath;
	t_split_err		split_err;
	t_get_fpath_err	get_fpath_err;

	cmd_args = split_improved(input, &split_err);
	if (split_err == SYNTAX_ERR)
		something();
	if (!cmd_args)
		malloc_failed();
	cmd_fullpath = get_fullpath(cmd_args, envp, &get_fpath_err);
	if (!cmd_fullpath)
	{

	}
	fork();
	execve(cmd_fullpath, cmd_args, envp);
	return (free(cmd_fullpath), free_2d_array(cmd_args), 0);
}

int	main(int argc, char **argv, char **envp)
{
	char *line;

	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			break;
		if (*line)
			add_history(line);
		exec_command(line, envp);
		free(line);
	}
	return (0);
}
