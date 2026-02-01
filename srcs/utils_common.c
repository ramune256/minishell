/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_common.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:18 by shunwata          #+#    #+#             */
/*   Updated: 2026/01/25 15:32:38 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_alloc *alloc)
{
	char	**tmp_ev;
	int		tmp_exit_status;
	bool	tmp_success;

	tmp_ev = alloc->ev_clone;
	tmp_exit_status = alloc->exit_status;
	tmp_success = alloc->success;
	free(alloc->line);
	free_tokens(alloc->head);
	free_ast(alloc->node);
	cleanup_temp_files(&alloc->temp_files);
	if (tmp_success == false)
		free_2d_array(&(alloc->ev_clone));
	ft_bzero(alloc, sizeof(t_alloc));
	if (tmp_success == true)
	{
		alloc->ev_clone = tmp_ev;
		alloc->exit_status = tmp_exit_status;
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

void	free_2d_array(char ***array)
{
	size_t	i;

	if (!array || !*array)
		return ;
	i = 0;
	while ((*array)[i])
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}
