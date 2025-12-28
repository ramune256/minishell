/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:30:58 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/18 16:15:33 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_exit_status(t_alloc *heap, int status)
{
	if (WIFEXITED(status))
		heap->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		heap->exit_status = 128 + WTERMSIG(status);
}
