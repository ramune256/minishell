/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:13:40 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/02 19:11:58 by nmasuda          ###   ########.fr       */
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

static char	**split_path(const char *s, char **result, size_t count)
{
	size_t	i;
	size_t	res_i;
	size_t	start;

	i = 0;
	res_i = 0;
	start = 0;
	while (res_i < count)
	{
		if (s[i] == ':' || s[i] == '\0')
		{
			result[res_i] = ft_substr(s, start, i - start);
			if (!result[res_i])
				return (free_2d_array(&result), NULL);
			res_i++;
			start = i + 1;
		}
		if (s[i++] == '\0')
			break ;
	}
	return (result);
}

char	**split_path_keep_empty(const char *s)
{
	char	**result;
	size_t	i;
	size_t	count;

	i = 0;
	count = 1;
	while (s[i])
	{
		if (s[i++] == ':')
			count++;
	}
	result = ft_calloc(count + 1, sizeof(char *));
	if (!result)
		return (NULL);
	return (split_path(s, result, count));
}
