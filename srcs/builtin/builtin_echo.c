/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:26 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 01:04:26 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static bool	is_n(char *str)
{
	int	i;

	if (!str || ft_strncmp(str, "-n", 2))
		return (false);
	i = 2;
	while (str[i] == 'n')
		i++;
	if (!str[i])
		return (true);
	return (false);
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
	else if (is_n(line[OPT]))
	{
		while (line[OPT] && is_n(line[OPT]))
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
