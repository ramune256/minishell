/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_signal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 19:21:32 by nmasuda           #+#    #+#             */
/*   Updated: 2026/03/04 22:02:14 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

void	import_signal_status(t_mshell *data)
{
	if (g_sig_status)
	{
		data->exit_status = 128 + SIGINT;
		g_sig_status = 0;
	}
}
