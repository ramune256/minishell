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
			ft_putstr_fd("_=/usr/bin/env\n", STDOUT_FILENO);
			i++;
		}
		else
		{
			ft_putstr_fd(heap->ev_clone[i++], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
	}
	return (0);
}
