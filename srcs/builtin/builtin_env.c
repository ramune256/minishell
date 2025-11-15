/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:37 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/14 20:00:17 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	c_env(char **line, t_alloc *heap)
{
	size_t	i;

	(void)line;
	if (!heap->new_ev)
		return ;
	i = 0;
	while (heap->new_ev[i])
	{
		if (!ft_strncmp(heap->new_ev[i], "_=", 2))
		{
			if (printf("_=/usr/bin/env\n") == ERROR)
				return (1);
		}
		else
			if (printf("%s\n", heap->new_ev[i++]) == ERROR)
				return (1);
	}
	return (0);
}
