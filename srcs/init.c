/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:36:55 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/14 18:51:51 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clone_ev(char **ev, t_alloc *heap)
{
	int	i;

	if (!ev)
		return ;
	i = 0;
	while (ev[i])
		i++;
	heap->new_ev = ft_calloc(i + 1, sizeof(char *));
	if (heap->new_ev)
		(cleanup(heap), exit(1));
	i = 0;
	while (ev[i])
	{
		heap->new_ev[i] = ft_strdup(ev[i]);
		if (!heap->new_ev[i])
			(cleanup(heap), exit(1));
		i++;
	}
	heap->new_ev[i] = NULL;
}
