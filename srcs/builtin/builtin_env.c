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
#include "builtin.h"

int	c_env(char **line, t_mshell *data)
{
	size_t	i;

	(void)line;
	if (!data->ev_clone)
		return (1);
	i = 0;
	while (data->ev_clone[i])
	{
		if (!ft_strncmp(data->ev_clone[i], "_=", 2))
		{
			ft_putstr_fd("_=/usr/bin/env\n", STDOUT_FILENO);
			i++;
		}
		else
		{
			ft_putstr_fd(data->ev_clone[i++], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
	}
	return (0);
}
