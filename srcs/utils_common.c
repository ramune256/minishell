/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_common.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:18 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/28 22:55:03 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_alloc *alloc)
{
	t_alloc	tmp;

	ft_bzero(&tmp, sizeof(t_alloc));
	tmp.ev_clone = alloc->ev_clone;
	tmp.exit_status = alloc->exit_status;
	tmp.success = alloc->success;
	tmp.av = alloc->av;
	tmp.ac = alloc->ac;
	(free(alloc->line), free_tokens(alloc->head), free_ast(alloc->node));
	cleanup_tmp_files(&alloc->tmp_files);
	if (tmp.success == false)
	{
		free_2d_array(&(alloc->ev_clone));
		rl_clear_history();
	}
	ft_bzero(alloc, sizeof(t_alloc));
	if (tmp.success == true)
	{
		alloc->ev_clone = tmp.ev_clone;
		alloc->exit_status = tmp.exit_status;
		alloc->av = tmp.av;
		alloc->ac = tmp.ac;
	}
}

void	get_input(char **line, const char *message)
{
	if (isatty(STDIN_FILENO))
		*line = readline(message);
	else
		*line = get_next_line(STDIN_FILENO);
}

void	print_exit(t_alloc *heap)
{
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
	cleanup(heap);
	exit(0);
}
