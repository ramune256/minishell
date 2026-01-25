/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:36:55 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/30 17:29:37 by shunwata         ###   ########.fr       */
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
	heap->ev_clone = ft_calloc(i + 1, sizeof(char *));
	if (!heap->ev_clone)
		(cleanup(heap), rl_clear_history(), exit(1));
	i = 0;
	while (ev[i])
	{
		heap->ev_clone[i] = ft_strdup(ev[i]);
		if (!heap->ev_clone[i])
			(cleanup(heap), rl_clear_history(), exit(1));
		i++;
	}
	heap->ev_clone[i] = NULL;
}
