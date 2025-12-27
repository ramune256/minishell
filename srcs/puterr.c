/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_err.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:10 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/28 01:32:17 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	puterr(char *cmd_name, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}
