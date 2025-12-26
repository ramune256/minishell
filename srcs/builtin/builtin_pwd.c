/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:02:45 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 01:05:39 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	c_pwd(char **line, t_alloc *heap)
{
	char	*tmp;

	(void)line;
	tmp = search_get_env(heap->ev_clone, "PWD");
	if (!tmp)
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
			return (1);
	}
	if (printf("%s\n", tmp) == ERROR)
		return (1);
	return (0);
}
