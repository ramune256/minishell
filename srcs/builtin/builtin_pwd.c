/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:02:45 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/16 17:47:32 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	c_pwd(char **line, t_alloc *heap)
{
	char	*tmp;

	(void)line;
//	(void)heap;
	tmp = serch_get_env(heap->new_ev, "PWD");
	if (!tmp)
		return (1);
	if (printf("%s\n", tmp) == ERROR)
		return (1);
	return (0);
}
