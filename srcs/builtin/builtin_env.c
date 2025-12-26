/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:37 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 02:15:15 by nmasuda          ###   ########.fr       */
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
			if (printf("_=/usr/bin/env\n") < 0)
				return (1);
			i++;
		}
		else
		{
			if (printf("%s\n", heap->ev_clone[i++]) < 0)
				return (1);
		}
	}
	return (0);
}
