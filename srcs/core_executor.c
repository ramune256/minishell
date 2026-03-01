/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:31 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/11 12:53:45 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

static t_cmd	*get_exec_node(t_cmd *node)
{
	while (node && node->type == NODE_REDIR)
		node = node->subcmd;
	return (node);
}

void	restore_stdio(int backups[2], t_alloc *heap)
{
	if (dup2(backups[STDIN_FILENO], STDIN_FILENO) == -1)
		(perror("dup2"), cleanup(heap), exit(1));
	if (dup2(backups[STDOUT_FILENO], STDOUT_FILENO) == -1)
		(perror("dup2"), cleanup(heap), exit(1));
	close(backups[STDIN_FILENO]);
	close(backups[STDOUT_FILENO]);
}

void	backup_stdio(int backups[2], t_alloc *heap)
{
	backups[STDIN_FILENO] = dup(STDIN_FILENO);
	backups[STDOUT_FILENO] = dup(STDOUT_FILENO);
	if (backups[STDIN_FILENO] == -1 || backups[STDOUT_FILENO] == -1)
		(perror("dup"), cleanup(heap), exit(1));
}

static bool	apply_parent_redirections(t_cmd *node, t_alloc *heap)
{
	t_cmd	*current;
	int		file_fd;

	current = node;
	while (current->type == NODE_REDIR)
	{
		file_fd = open(current->file, current->mode, 0644);
		if (file_fd == -1)
			return (perror(current->file), false);
		if (dup2(file_fd, current->fd) == -1)
			return (perror("dup2"), close(file_fd), false);
		close(file_fd);
		current = current->subcmd;
	}
	return (true);
}

static void	execute_parent_builtin(t_cmd *node, t_alloc *heap)
{
	int		backups[2];

	backup_stdio(backups, heap);
	if (apply_parent_redirections(node, backups, heap) == false)
	{
		restore_stdio(backups, heap);
		heap->exit_status = 1;
		cleanup_tmp_files(&heap->tmp_files);
		return ;
	}
	execute_builtin(get_exec_node(node), heap);
	restore_stdio(backups, heap);
	cleanup_tmp_files(&heap->tmp_files);
}

static void	execute_command(t_cmd *node, t_alloc *heap)
{
	t_cmd	*exec_node;
	char	*fullpath;
	int		tmp_exit_status;

	set_signal_child();
	exec_node = handle_redirections(node, heap);
	if (execute_builtin(exec_node, heap))
		(cleanup(heap), exit(heap->exit_status));
	if ((!exec_node->argv))
		(cleanup(heap), exit(heap->exit_status));
	fullpath = get_fullpath(exec_node->argv[0], heap);
	if (fullpath == NULL)
	{
		tmp_exit_status = heap->exit_status;
		(cleanup(heap), exit(tmp_exit_status));
	}
	if (execve(fullpath, exec_node->argv, heap->ev_clone) == -1)
		(perror(fullpath), free(fullpath), cleanup(heap), exit(126));
}

static void	execute_exec(t_cmd *node, t_alloc *heap)
{
	pid_t	pid;
	int		status;

	if (g_sig_status)
		return ;
	pid = fork();
	if (pid == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid == 0)
		execute_command(node, heap);
	set_signal_parent();
	waitpid(pid, &status, 0);
	set_signal_shell();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		ft_putstr_fd("\n", STDOUT_FILENO);
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	get_exit_status(heap, status);
	cleanup_tmp_files(&heap->tmp_files);
}

static void	execute_pipe(t_cmd *node, t_alloc *heap)
{
	int		status;
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (node->type == NODE_EXEC)
	{
		execute_exec(node, heap);
		return ;
	}
	if (pipe(pipefd) == -1)
		(perror("pipe"), cleanup(heap), exit(1));
	pid_left = execute_subnode(node->left, pipefd, STDOUT_FILENO, heap);
	pid_right = execute_subnode(node->right, pipefd, STDIN_FILENO, heap);
	(close(pipefd[0]), close(pipefd[1]));
	set_signal_parent();
	(waitpid(pid_left, NULL, 0), waitpid(pid_right, &status, 0));
	set_signal_shell();
	get_exit_status(heap, status);
}

void	execute(t_cmd *node, t_alloc *heap)
{
	if (node == NULL)
		return ;
	if (node->type == NODE_PIPE)
		execute_pipe(node, heap);
	else if (node->type == NODE_REDIR)
	{
		if (is_parent_builtin(get_exec_node(node)))
			execute_parent_builtin(node, heap);
		else
			execute_exec(node, heap);
	}
	else if (node->type == NODE_EXEC)
	{
		if (is_parent_builtin(node))
			execute_builtin(node, heap);
		else
			execute_exec(node, heap);
	}
}
