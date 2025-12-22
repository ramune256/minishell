/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/22 19:34:57 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	c_exit(char **line, t_alloc *heap)
{
	long long	arg;
	int			status;
	bool		error;

	if (isatty(STDIN_FILENO))
		fprintf(stderr, "exit\n");
	if (!line || !line[1])
	{
		status = heap->exit_status;
		(cleanup(heap), exit(status));
	}
	error = false;
	arg = ft_atol(line[1], &error);
	if (error)
		(fprintf(stderr, "minishell: exit: %s: numeric argument required\n", line[1]), cleanup(heap), exit(255));
	if (line[2])
		return (fprintf(stderr, "minishell: exit: too many arguments\n"), 1); // 終了しない
	cleanup(heap);
	exit((unsigned char)arg); //モジュロ演算
	return (0); //到達しない
}
