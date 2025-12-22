/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:37 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/22 22:25:41 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	c_env(char **line, t_alloc *heap)
{
	size_t	i;

	(void)line;
	if (!heap->ev_clone)
		return (1);
	i = 0;
	while (heap->ev_clone[i])
	{
		if (!ft_strncmp(heap->ev_clone[i], "_=", 2))
		{
			if (printf("_=/usr/bin/env\n") == ERROR)
				return (1);
		}
		else
		{
			if (printf("%s\n", heap->ev_clone[i++]) == ERROR)
				return (1);
		}
	}
	return (0);
}
