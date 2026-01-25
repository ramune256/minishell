/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:26 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 01:40:48 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	echo_print(char **line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		ft_putstr_fd(line[i++], STDOUT_FILENO);
		if (line[i])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	return (true);
}

static bool	is_n_opt(const char *arg)
{
	int	i;

	if (!arg || ft_strncmp(arg, "-n", 2))
		return (false);
	i = ft_strlen("-n");
	while (arg[i] == 'n')
		i++;
	if (arg[i] != '\0')
		return (false);
	return (true);
}

int	c_echo(char **line, t_alloc *heap)
{
	int	i;

	i = 1;
	(void)heap;
	if (!line)
		return (1);
	if (!line[1])
		ft_putstr_fd("\n", STDOUT_FILENO);
	else if (is_n_opt(line[1]))
	{
		while (line[i] && is_n_opt(line[i]))
			i++;
		if (echo_print(line + i) == false)
			return (1);
	}
	else
	{
		if (echo_print(line + 1) == false)
			return (1);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (0);
}
