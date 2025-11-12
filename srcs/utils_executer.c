/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:30:58 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/12 17:52:34 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_fd(int pipefd[2], int target_fd, int fd_num)
{
	dup2(fd_num, target_fd);
	close(pipefd[0]);
	close(pipefd[1]);
}
