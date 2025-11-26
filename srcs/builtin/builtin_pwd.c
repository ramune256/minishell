/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:02:45 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/20 15:00:47 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	c_pwd(char **line, t_alloc *heap)
{
	char	*tmp;

	(void)line;
	tmp = serch_get_env(heap->ev_clone, "PWD");
	if (!tmp)
		return (1);
	if (printf("%s\n", tmp) == ERROR)
		return (1);
	return (0);
}
