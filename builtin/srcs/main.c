/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:46:38 by nmasuda           #+#    #+#             */
/*   Updated: 2025/10/20 15:31:57 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

void	built_in_check(char **line, char **ev)
{
	char	**res;

	res = NULL;
	if (!ft_strncmp(line[CMD], "echo", 5))
		c_echo(line);
	else if (!ft_strncmp(line[CMD], "pwd", 4))
		c_pwd(line);
	else if (!ft_strncmp(line[CMD], "env", 4))
		c_env(line, ev);
	else if (!ft_strncmp(line[CMD], "cd", 3))
		res = c_cd(line, ev);
	else if (!ft_strncmp(line[CMD], "exit", 5))
		c_exit(line);
	else if (!ft_strncmp(line[CMD], "unset", 6))
		res = c_unset(line, ev);
	else if (!ft_strncmp(line[CMD], "export", 7))
	{
		res = c_export(line, ev);
		c_check(line[0], res);
	}
}

int	main(int ac, char **av, char **ev)
{
	(void)ac;
	built_in_check(av, ev);
	return (0);
}
