/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:30:58 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/04 18:30:38 by nmasuda          ###   ########.fr       */
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

static void	init(const char *s, size_t *j, size_t *start, size_t *count)
{
	size_t	i;

	i = 0;
	*j = 0;
	*start = 0;
	*count = 1;

	while (s[i++])
	{
		if (s[i] == ':')
			count++;
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
			res[j++] = ft_substr(s, start, i - start);
			start = i + 1;
		}
		if (s[i] == '\0')
			break ;
		i++;
	}
	return (res);
}
