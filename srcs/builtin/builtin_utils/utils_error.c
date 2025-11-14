/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:54:33 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/14 19:10:03 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	error(char *bash, char *mess, char **line, int exit_num)
{
	if (line)
		free_all(line);
	if (bash)
		ft_putstr_fd(bash, exit_num);
	if (mess)
		ft_putstr_fd(mess, exit_num);
	exit(exit_num);
}
