/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:30:58 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/04 01:51:41 by nmasuda          ###   ########.fr       */
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

char	**split_path_keep_empty(const char *s)
{
	char	**res;
	size_t	i = 0;
	size_t	j = 0;
	size_t	start = 0;
	size_t	count = 1;

	while (s[i])
	{
		if (s[i] == ':')
			count++;
		i++;
	}
	res = ft_calloc(count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (j < count)
	{
		if (s[i] == ':' || s[i] == '\0')
		{
			res[j] = ft_substr(s, start, i - start);
			j++;
			start = i + 1;
		}
		if (s[i] == '\0')
			break ;
		i++;
	}
	return (res);
}
