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

static char	**split_path(const char *s, char **res, size_t count)
{
	size_t	s_cur;
	size_t	res_cur;
	size_t	start;

	s_cur = 0;
	res_cur = 0;
	start = 0;
	while (res_cur < count)
	{
		if (s[s_cur] == ':' || s[s_cur] == '\0')
		{
			res[res_cur] = ft_substr(s, start, s_cur - start);
			if (!res[res_cur])
				return (free_2d_array(&res), NULL);
			res_cur++;
			start = s_cur + 1;
		}
		if (s[s_cur++] == '\0')
			break ;
	}
	return (res);
}

char	**split_path_keep_empty(const char *s)
{
	char	**res;
	size_t	s_cur;
	size_t	count;

	s_cur = 0;
	count = 1;
	while (s[s_cur])
	{
		if (s[s_cur++] == ':')
			count++;
	}
	res = ft_calloc(count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	return (split_path(s, res, count));
}
