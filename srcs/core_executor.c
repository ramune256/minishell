/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:31 by shunwata          #+#    #+#             */
/*   Updated: 2026/01/28 18:19:28 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*handle_redirections(t_cmd *node, t_alloc *heap)
{
	t_cmd	*exec_node;
	int		file_fd;

	if (node->type == NODE_EXEC)
		return (node);
	exec_node = handle_redirections(node->subcmd, heap);
	file_fd = open(node->file, node->mode, 0644);
	if (file_fd == -1)
		(perror(node->file), cleanup(heap), exit(1));
	if (dup2(file_fd, node->fd) == -1)
		(perror("dup2"), cleanup(heap), exit(1));
	close(file_fd);
	return (exec_node);
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

static void    execute_exec(t_cmd *node, t_alloc *heap)
{
    pid_t    pid;
    int        status;

    //if (node->type != NODE_PIPE)
    //    find_and_process_heredocs(node, heap);
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
    cleanup_temp_files(&heap->temp_files);
}

static void	execute_pipe(t_cmd *node, t_alloc *heap)
{
	int		status;
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (node->type == NODE_EXEC)
		execute_exec(node, heap);
	if (pipe(pipefd) == -1)
		(perror("pipe"), cleanup(heap), exit(1));
	pid_left = fork();
	if (pid_left == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid_left == 0)
	{
		set_signal_child();
		dup2(pipefd[1], STDOUT_FILENO);
		(close(pipefd[0]), close(pipefd[1]));
		execute(node->left, heap);
		(cleanup(heap), exit(heap->exit_status));
	}
	pid_right = fork();
	if (pid_right == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid_right == 0)
	{
		set_signal_child();
		dup2(pipefd[0], STDIN_FILENO);
		(close(pipefd[0]), close(pipefd[1]));
		execute(node->right, heap);
		(cleanup(heap), exit(heap->exit_status));
	}
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
		execute_exec(node, heap);
	else if (node->type == NODE_EXEC)
	{
		if (is_parent_builtin(node))
			execute_builtin(node, heap);
		else
			execute_exec(node, heap);
	}
}
