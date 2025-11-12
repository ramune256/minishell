/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:54:33 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/10 01:54:54 by nmasuda          ###   ########.fr       */
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
