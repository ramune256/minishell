/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:54:33 by nmasuda           #+#    #+#             */
/*   Updated: 2025/10/20 15:31:57 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

void	error(char *bash, char *mess, char **line, int exit_num)
{
	int	i;

	i = 0;
	if (line)
		free_all(line);
	if (bash)
		ft_putstr_fd(bash, exit_num);
	if (mess)
		ft_putstr_fd(mess, exit_num);
	exit(exit_num);
}
