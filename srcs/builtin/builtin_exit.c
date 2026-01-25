/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/22 22:25:47 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	c_exit(char **line, t_alloc *heap)
{
	long long	arg;
	bool		error;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
	if (!line || !line[1])
		(cleanup(heap), rl_clear_history(), exit(heap->exit_status));
	error = false;
	arg = ft_atol(line[1], &error);
	if (error)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(line[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		(cleanup(heap), rl_clear_history(), exit(255));
	}
	if (line[2])
		return (puterr("exit", "too many arguments"), 1);
	cleanup(heap);
	rl_clear_history();
	exit((unsigned char)arg);
	return (0);
}
