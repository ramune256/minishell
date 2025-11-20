/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:31 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/20 14:59:02 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	execute_simple_command(t_cmd *ast, t_alloc *heap)
{
	pid_t	pid;
	t_cmd	*exec_node;
	char	*fullpath;
	int		status;

	if (ast->type != NODE_PIPE)
		find_and_process_heredocs(ast, heap);
	pid = fork();
	if (pid == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid == 0)
	{
		exec_node = handle_redirections(ast, heap);
		if (execute_builtin(exec_node, heap))
			(cleanup(heap), exit(heap->exit_status));
		fullpath = get_fullpath(exec_node->argv[0], heap);
		if (fullpath == NULL)
			(cleanup(heap), exit(127));
		if (execve(fullpath, exec_node->argv, heap->ev_clone) == -1)
			(perror(fullpath), free(fullpath), cleanup(heap), exit(126));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status)) //ステータスを解析
		heap->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		heap->exit_status = 128 + WTERMSIG(status);
	cleanup_temp_files(&heap->temp_files);
}

static void	execute_pipe(t_cmd *ast, t_alloc *heap)
{
	int		pipefd[2]; // [0]は読み込み口, [1]は書き込み口
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(pipefd) == -1)
		(perror("pipe"), cleanup(heap), exit(1)); // パイプ作成失敗は致命的
	pid_left = fork(); // 1. 左側の子プロセス (パイプに書き込む側)
	if (pid_left == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid_left == 0)
	{
		change_fd(pipefd, STDOUT_FILENO, pipefd[1]);
		execute(ast->left, heap); // 左側のASTを再帰的に実行
		(cleanup(heap), exit(heap->exit_status)); // 子プロセスをクリーンアップ
	}
	pid_right = fork(); // 2. 右側の子プロセス (パイプから読み込む側)
	if (pid_right == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid_right == 0)
	{
		change_fd(pipefd, STDIN_FILENO, pipefd[0]);
		execute(ast->right, heap); // 右側のASTを再帰的に実行
		(cleanup(heap), exit(heap->exit_status)); // 子プロセスをクリーンアップ
	}
	(close(pipefd[0]), close(pipefd[1]));
	(waitpid(pid_left, NULL, 0), waitpid(pid_right, &status, 0));
	if (WIFEXITED(status))
		heap->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		heap->exit_status = 128 + WTERMSIG(status);
}

// ASTを再帰的にたどって実行するエントリーポイント
void	execute(t_cmd *ast, t_alloc *heap)
{
	if (ast == NULL)
		return ;
	if (ast->type == NODE_PIPE)
		execute_pipe(ast, heap);
	else if (ast->type == NODE_REDIR)
		execute_simple_command(ast, heap);
	else if (ast->type == NODE_EXEC)
	{
		if (is_parent_builtin(ast))
			execute_builtin(ast, heap); // forkせずに、親プロセスでそのまま実行
		else
			execute_simple_command(ast, heap); // forkする
	}
}
