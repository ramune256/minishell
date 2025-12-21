/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:30:58 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/22 00:15:00 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_fd(int pipefd[2], int target_fd, int fd_num)
{
	dup2(fd_num, target_fd);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	ft_perror(char *cmd, char *mess)
{
	ft_putendl_fd("bash: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(mess, 2);
}
