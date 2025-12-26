/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:31 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/21 16:23:35 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*handle_redirections(t_cmd *ast, t_alloc *heap)
{
	t_cmd	*exec_node;
	int		file_fd;

	if (ast->type == NODE_EXEC) // ベースケース：EXECノードに到達したら、それを返す
		return (ast);
	exec_node = handle_redirections(ast->subcmd, heap); //複数リダイレクトを想定
	file_fd = open(ast->file, ast->mode, 0644); // 0644 = rw-r--r--
	if (file_fd == -1)
		(perror(ast->file), cleanup(heap), exit(1)); // 子プロセスを終了
	if (dup2(file_fd, ast->fd) == -1)
		(perror("dup2"), cleanup(heap), exit(1));
	close(file_fd); // dup2したので、元のFDは不要
	return (exec_node);
}

static void	execute_single_command(t_cmd *ast, t_alloc *heap)
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
		set_signal_child();
		exec_node = handle_redirections(ast, heap);
		if (execute_builtin(exec_node, heap))
			clean_exit(heap, heap->exit_status);
		fullpath = get_fullpath(exec_node->argv[0], heap);
		if (fullpath == NULL)
			(cleanup(heap), exit(127));
		if (execve(fullpath, exec_node->argv, heap->ev_clone) == -1)
			(perror(fullpath), free(fullpath), cleanup(heap), exit(126));
	}
	set_signal_parent();
	waitpid(pid, &status, 0);
	set_signal_shell();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		ft_putstr_fd("\n", STDOUT_FILENO);
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	get_exit_status(heap, status);
	cleanup_temp_files(&heap->temp_files);
}

static void	exec_pipe_child(t_cmd *cmd, int fd, int std, int *pfd, t_alloc *heap)
{
	set_signal_child();
	dup2(fd, std);
	(close(pfd[0]), close(pfd[1]));
	execute(cmd, heap);
	clean_exit(heap, heap->exit_status);
}

static void	execute_pipe(t_cmd *ast, t_alloc *heap)
{
	int		pfd[2];
	pid_t	l;
	pid_t	r;
	int		s;

	if (pipe(pfd) == -1)
		(perror("pipe"), clean_exit(heap, 1));
	l = fork();
	if (l == -1)
		(perror("fork"), clean_exit(heap, 1));
	if (l == 0)
		exec_pipe_child(ast->left, pfd[1], STDOUT_FILENO, pfd, heap);
	r = fork();
	if (r == -1)
		(perror("fork"), clean_exit(heap, 1));
	if (r == 0)
		exec_pipe_child(ast->right, pfd[0], STDIN_FILENO, pfd, heap);
	(close(pfd[0]), close(pfd[1]));
	set_signal_parent();
	(waitpid(l, NULL, 0), waitpid(r, &s, 0));
	set_signal_shell();
	get_exit_status(heap, s);
}

// ASTを再帰的にたどって実行するエントリーポイント
void	execute(t_cmd *ast, t_alloc *heap)
{
	if (ast == NULL)
		return ;
	if (ast->type == NODE_PIPE)
		execute_pipe(ast, heap);
	else if (ast->type == NODE_REDIR)
		execute_single_command(ast, heap);
	else if (ast->type == NODE_EXEC)
	{
		if (is_parent_builtin(ast))
			execute_builtin(ast, heap); // forkせずに、親プロセスでそのまま実行
		else
			execute_single_command(ast, heap); // forkする
	}
}
