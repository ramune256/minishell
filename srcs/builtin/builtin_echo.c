/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:26 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/30 16:13:09 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static bool	echo_print(char **line)
{
	int	i;

	i = CMD + 1;
	while (line[i])
	{
		if (printf("%s", line[i++]) == ERROR)
			return (false);
		if (line[i])
		{
			if (printf(" ") == ERROR)
				return (false);
		}
	}
	return (true);
}

int	c_echo(char **line, t_alloc *heap)
{
	(void)heap;
	if (!line)
		return (1);
	if (!line[OPT])
	{
		if (printf("\n") == ERROR)
			return (1);
	}
	else if (!ft_strncmp(line[OPT], "-n", 3))
	{
		while (!ft_strncmp(line[OPT], "-n", 3))
			line++;
		if (echo_print(line) == false)
			return (1);
	}
	else
	{
		if (echo_print(line) == false)
			return (1);
		if (printf("\n") == ERROR)
			return (1);
	}
	return (0);
}
