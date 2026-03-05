/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_common.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:18 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:00:14 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_mshell *data)
{
	t_mshell	tmp;

	ft_bzero(&tmp, sizeof(t_mshell));
	tmp.ev_clone = data->ev_clone;
	tmp.exit_status = data->exit_status;
	tmp.success = data->success;
	tmp.av = data->av;
	tmp.ac = data->ac;
	(free(data->line), free_tokens(data->head), free_ast(data->node));
	cleanup_tmp_files(&data->tmp_files);
	if (tmp.success == false)
	{
		free_2d_array(&(data->ev_clone));
		rl_clear_history();
	}
	ft_bzero(data, sizeof(t_mshell));
	if (tmp.success == true)
	{
		data->ev_clone = tmp.ev_clone;
		data->exit_status = tmp.exit_status;
		data->av = tmp.av;
		data->ac = tmp.ac;
	}
}

void	get_input(char **line, const char *message)
{
	if (isatty(STDIN_FILENO))
		*line = readline(message);
	else
		*line = get_next_line(STDIN_FILENO);
}

void	print_exit(t_mshell *data)
{
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
	cleanup(data);
	exit(0);
}
