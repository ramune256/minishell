/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:31 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/28 01:50:05 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	branch_pipe_or_child_task(t_cmd *node, t_alloc *heap);

static t_cmd	*handle_redirections(t_cmd *ast, t_alloc *heap)
{
	t_cmd	*exec_node;
	int		file_fd;

	if (ast->type == NODE_EXEC)
		return (ast);
	exec_node = handle_redirections(ast->subcmd, heap);
	file_fd = open(ast->file, ast->mode, 0644);
	if (file_fd == -1)
		(perror(ast->file), cleanup_and_exit(heap, 1));
	if (dup2(file_fd, ast->fd) == -1)
		(perror("dup2"), cleanup_and_exit(heap, 1));
	close(file_fd);
	return (exec_node);
}

void	execute_child_task(t_cmd *node, t_alloc *heap)
{
	t_cmd	*exec_node;
	char	*fullpath;

	set_signal_child();
	exec_node = handle_redirections(node, heap);
	if (execute_builtin(exec_node, heap))
		cleanup_and_exit(heap, heap->exit_status);
	fullpath = get_fullpath(exec_node->argv[0], heap);
	if (fullpath == NULL)
		cleanup_and_exit(heap, 127);
	if (execve(fullpath, exec_node->argv, heap->ev_clone) == -1)
		(perror(fullpath), free(fullpath), cleanup_and_exit(heap, 126));
}

static void	execute_single_command(t_cmd *ast, t_alloc *heap)
{
	pid_t	pid;
	int		status;

	if (ast->type != NODE_PIPE)
		find_and_process_heredocs(ast, heap);
	pid = fork();
	if (pid == -1)
		(perror("fork"), cleanup_and_exit(heap, 1));
	if (pid == 0)
		execute_child_task(ast, heap);
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

static void	execute_pipe(t_cmd *ast, t_alloc *heap)
{
	int		status;
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(pipefd) == -1)
		(perror("pipe"), cleanup_and_exit(heap, 1));
	pid_left = fork();
	if (pid_left == -1)
		(perror("fork"), cleanup_and_exit(heap, 1));
	if (pid_left == 0)
	{
		set_signal_child();
		dup2(pipefd[1], STDOUT_FILENO);
		(close(pipefd[0]), close(pipefd[1]));
		branch_pipe_or_child_task(ast->left, heap);
		cleanup_and_exit(heap, heap->exit_status);
	}
	pid_right = fork();
	if (pid_right == -1)
		(perror("fork"), cleanup_and_exit(heap, 1));
	if (pid_right == 0)
	{
		set_signal_child();
		dup2(pipefd[0], STDIN_FILENO);
		(close(pipefd[0]), close(pipefd[1]));
		branch_pipe_or_child_task(ast->right, heap);
		cleanup_and_exit(heap, heap->exit_status);
	}
	(close(pipefd[0]), close(pipefd[1]));
	set_signal_parent();
	(waitpid(pid_left, NULL, 0), waitpid(pid_right, &status, 0));
	set_signal_shell();
	get_exit_status(heap, status);
}

static void	branch_pipe_or_child_task(t_cmd *node, t_alloc *heap)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
		execute_pipe(node, heap);
	else
		execute_child_task(node, heap);
}

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
			execute_builtin(ast, heap);
		else
			execute_single_command(ast, heap);
	}
}
