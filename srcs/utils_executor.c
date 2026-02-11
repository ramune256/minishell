/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:30:58 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/11 12:53:59 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_exit_status(t_alloc *heap, int status)
{
	if (WIFEXITED(status))
		heap->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		heap->exit_status = 128 + WTERMSIG(status);
}

t_cmd	*handle_redirections(t_cmd *node, t_alloc *heap)
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

void	set_pipeend(int pipefd[2], int dest_fd, t_alloc *heap)
{
	int	src_fd;

	if (dest_fd == STDOUT_FILENO)
		src_fd = pipefd[1];
	else
		src_fd = pipefd[0];
	if (dup2(src_fd, dest_fd) == -1)
		(perror("dup2"), cleanup(heap), exit(1));
	(close(pipefd[0]), close(pipefd[1]));
}

pid_t	execute_subnode(t_cmd *node, int pipefd[2], int dest_fd, t_alloc *heap)
{
	pid_t	child;

	child = fork();
	if (child == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (child == 0)
	{
		set_signal_child();
		set_pipeend(pipefd, dest_fd, heap);
		execute(node, heap);
		(cleanup(heap), exit(heap->exit_status));
	}
	return (child);
}
