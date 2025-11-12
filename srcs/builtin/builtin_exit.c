/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/10 16:07:35 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	echo_exit(char **line, int exit_num, char *mess, int many_arg)
{
	printf("exit\n");
	if (mess)
		printf("%s: exit: %s: numeric argument required\n", line[CMD + 1],
			mess);
	if (many_arg)
		printf("%s: exit: too many arguments\n", line[0]);
	exit(exit_num);
}

char	**c_exit(char **line, char **ev)
{
	int			error;
	long long	num;

	error = 0;
	if (line[CMD + 1])
	{
		num = ft_atoi(line[CMD + 1], &error);
		if (error)
			echo_exit(line, 2, line[CMD + 1], 0);
		if (line[CMD + 2])
			echo_exit(line, 1, NULL, 1);
		while (num < 0)
			num += 256;
		num = num % 256;
		echo_exit(line, num, NULL, 0);
	}
	else
	{
		echo_exit(NULL, 0, NULL, 0);
	}
	return (ev);
}
