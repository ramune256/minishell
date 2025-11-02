/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_executer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:31 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/31 21:48:48 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// // NODE_EXEC を実行する
// static void	execute_exec(t_alloc *heap, t_cmd *ast, char **envp)
// {
// 	pid_t	pid;
// 	char	*fullpath;

// 	fullpath = get_fullpath(ast->argv[0], envp, heap);
// 	pid = fork();
// 	if (pid == -1)
// 		(perror("fork"), cleanup(heap), exit(1));
// 	if (pid == 0)
// 	{
// 		if (execve(fullpath, ast->argv, envp) == -1)
// 			(perror(ast->argv[0]), exit(127)); //not foundは127
// 	}
// 	waitpid(pid, NULL, 0); // TODO: 終了ステータスを受け取る
// }

// static void	execute_exec(t_cmd *ast, t_alloc *heap, char **ev)
// {
// 	pid_t	pid;
// 	char	*fullpath;

// 	pid = fork();
// 	if (pid == -1)
// 		(perror("fork"), cleanup(heap), exit(1));
// 	if (pid == 0)
// 	{
// 		fullpath = get_fullpath(ast->argv[0], ev, heap);
// 		if (fullpath == NULL)
// 			(cleanup(heap), exit(127));
// 		if (execve(fullpath, ast->argv, ev) == -1)
// 			(perror(ast->argv[0]), cleanup(heap), exit(126));
// 	}
// 	waitpid(pid, NULL, 0); // 親プロセスは待つだけ
// }

static t_cmd	*handle_redirections(t_cmd *ast, t_alloc *heap)
{
	int	file_fd;

	if (ast->type == NODE_EXEC) // ベースケース：EXECノードに到達したら、それを返す
		return (ast);
	if (ast->subcmd->type == NODE_REDIR) // ASTが REDIR(REDIR(EXEC)) の場合、再帰的に処理
		handle_redirections(ast->subcmd, heap);
	file_fd = open(ast->file, ast->mode, 0644); // 0644 = rw-r--r--
	if (file_fd == -1)
		(perror(ast->file), cleanup(heap), exit(1)); // 子プロセスを終了
	if (dup2(file_fd, ast->fd) == -1)
		(perror("dup2"), cleanup(heap), exit(1));
	close(file_fd); // dup2したので、元のFDは不要
	return (ast->subcmd);
}

static void	execute_simple_command(t_cmd *ast, t_alloc *heap, char **ev)
{
	pid_t	pid;
	t_cmd	*exec_node;
	char	*fullpath;

	pid = fork();
	if (pid == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid == 0)
	{
		exec_node = handle_redirections(ast, heap);
		fullpath = get_fullpath(exec_node->argv[0], ev, heap);
		if (fullpath == NULL)
			(cleanup(heap), exit(127));
		if (execve(fullpath, exec_node->argv, ev) == -1)
			(perror(fullpath), free(fullpath), cleanup(heap), exit(126));
	}
	waitpid(pid, NULL, 0);
}

static void	change_fd(int pipefd[2], int target_fd, int fd_num)
{
	dup2(fd_num, target_fd);
	close(pipefd[0]);
	close(pipefd[1]);
}

static void	execute_pipe(t_cmd *ast, t_alloc *heap, char **envp)
{
	int		pipefd[2]; // [0]は読み込み口, [1]は書き込み口
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(pipefd) == -1)
		(perror("pipe"), cleanup(heap), exit(1)); // パイプ作成失敗は致命的
	pid_left = fork(); // 1. 左側の子プロセス (パイプに書き込む側)
	if (pid_left == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid_left == 0)
	{
		change_fd(pipefd, STDOUT_FILENO, pipefd[1]);
		execute(ast->left, heap, envp); // 左側のASTを再帰的に実行
		(cleanup(heap), exit(0)); // 子プロセスをクリーンアップ
	}
	pid_right = fork(); // 2. 右側の子プロセス (パイプから読み込む側)
	if (pid_right == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid_right == 0)
	{
		change_fd(pipefd, STDIN_FILENO, pipefd[0]);
		execute(ast->right, heap, envp); // 右側のASTを再帰的に実行
		(cleanup(heap), exit(0)); // 子プロセスをクリーンアップ
	}
	(close(pipefd[0]), close(pipefd[1]));
	(waitpid(pid_left, NULL, 0), waitpid(pid_right, NULL, 0)); // TODO: 終了ステータス
}

// ASTを再帰的にたどって実行するエントリーポイント
void	execute(t_cmd *ast, t_alloc *heap, char **ev)
{
	if (ast == NULL)
		return ;
	if (ast->type == NODE_PIPE)
		execute_pipe(ast, heap, ev);
	else if (ast->type == NODE_EXEC || ast->type == NODE_REDIR)
		execute_simple_command(ast, heap, ev);
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
