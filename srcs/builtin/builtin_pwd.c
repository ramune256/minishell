/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:02:45 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 01:41:30 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

int	c_pwd(char **line, t_mshell *data)
{
	char	*tmp;

	(void)line;
	tmp = search_get_env(data->ev_clone, "PWD");
	if (!tmp)
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
			return (1);
	}
	ft_putstr_fd(tmp, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
