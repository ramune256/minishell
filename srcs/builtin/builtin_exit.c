/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/21 15:24:55 by shunwata         ###   ########.fr       */
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

// static void	echo_exit(char **line, int exit_num, char *mess, int many_arg)
// {
// 	printf("exit\n");
// 	if (mess)
// 		printf("%s: exit: %s: numeric argument required\n", line[CMD + 1],
// 			mess);
// 	if (many_arg)
// 		printf("%s: exit: too many arguments\n", line[0]);
// 	exit(exit_num);
// }

// int	c_exit(char **line, t_alloc *heap)
// {
// 	int			error;
// 	long long	num;

// 	error = 0;
// 	if (!line)
// 		return (1);
// 	if (line[CMD + 1])
// 	{
// 		num = ft_atol(line[CMD + 1], &error);
// 		if (error)
// 			echo_exit(line, 2, line[CMD + 1], 0);
// 		if (line[CMD + 2])
// 			echo_exit(line, 1, NULL, 1);
// 		while (num < 0)
// 			num += 256;
// 		num = num % 256;
// 		echo_exit(line, num, NULL, 0);
// 	}
// 	else
// 		echo_exit(NULL, 0, NULL, 0);
// 	return (0); // ←おそらく到達しない
// }
