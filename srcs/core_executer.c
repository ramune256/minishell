/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_executer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:31 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/29 17:28:52 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NODE_EXEC を実行する
static void	execute_exec(t_alloc *heap, t_cmd *ast, char **envp)
{
	pid_t	pid;
	char	*fullpath;

	fullpath = get_fullpath(ast->argv[0], envp, heap);
	pid = fork();
	if (pid == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid == 0)
	{
		if (execve(fullpath, ast->argv, envp) == -1)
			(perror(ast->argv[0]), exit(127)); //not foundは127
	}
	waitpid(pid, NULL, 0); // TODO: 終了ステータスを受け取る
}

// ASTを再帰的にたどって実行するエントリーポイント
void	execute(t_alloc *heap, char **envp)
{
	t_cmd	*ast;

	ast = heap->ast;
	if (ast == NULL)
		return ;
	if (ast->type == NODE_EXEC)
		execute_exec(heap, ast, envp);
	else if (ast->type == NODE_PIPE)
	{
		printf("Pipe execution is not implemented yet.\n");
		// TODO: execute_pipe(ast, envp);
	}
	else if (ast->type == NODE_REDIR)
	{
		printf("Redirection is not implemented yet.\n");
		// TODO: execute_redir(ast, envp);
	}
}

// int	exec_command(char *input, char **envp)
// {
// 	pid_t			pid;
// 	char			**cmd_args;
// 	char			*cmd_fullpath;
// 	t_split_err		split_err;
// 	t_get_fpath_err	get_fpath_err;

// 	cmd_args = split_improved(input, &split_err);
// 	if (split_err == SYNTAX_ERR)
// 		something();
// 	if (!cmd_args)
// 		malloc_failed();
// 	cmd_fullpath = get_fullpath(cmd_args, envp, &get_fpath_err);
// 	if (!cmd_fullpath)
// 	{

// 	}
// 	fork();
// 	execve(cmd_fullpath, cmd_args, envp);
// 	return (free(cmd_fullpath), free_2d_array(cmd_args), 0);
// }
