/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:13:40 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/25 20:13:43 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

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

static void	init(const char *s, size_t *j, size_t *start, size_t *count)
{
	size_t	i;

	i = 0;
	*j = 0;
	*start = 0;
	*count = 1;

	while (s[i])
	{
		if (s[i] == ':')
			(*count)++;
		i++;
	}
}

char	**split_path_keep_empty(const char *s)
{
	char	**res;
	size_t	i;
	size_t	j;
	size_t	start;
	size_t	count;

	init(s, &j, &start, &count);
	res = ft_calloc(count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (j < count)
	{
		if (s[i] == ':' || s[i] == '\0')
		{
			res[j] = ft_substr(s, start, i - start);
			if(!res[j])
				return (free_2d_array(&res), NULL);
			j++;
			start = i + 1;
		}
		if (s[i] == '\0')
			break ;
		i++;
	}
	return (res);
}
