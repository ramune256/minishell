/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:36:55 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:00:38 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clone_ev(char **ev, t_mshell *data)
{
	int	i;

	if (!ev)
		return ;
	i = 0;
	while (ev[i])
		i++;
	data->ev_clone = ft_calloc(i + 1, sizeof(char *));
	if (!data->ev_clone)
		(cleanup(data), exit(1));
	i = 0;
	while (ev[i])
	{
		data->ev_clone[i] = ft_strdup(ev[i]);
		if (!data->ev_clone[i])
			(cleanup(data), exit(1));
		i++;
	}
	data->ev_clone[i] = NULL;
}
