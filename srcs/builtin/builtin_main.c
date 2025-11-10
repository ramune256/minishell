/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:46:38 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/10 17:26:09 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

char	**built_in_check(char **line, char **ev)
{
	char	**new_ev;

	new_ev = NULL;
	if (!ft_strncmp(line[CMD], "echo", 5))
		new_ev = c_echo(line, ev);
	else if (!ft_strncmp(line[CMD], "pwd", 4))
		new_ev = c_pwd(line, ev);
	else if (!ft_strncmp(line[CMD], "env", 4))
		new_ev = c_env(line, ev);
	else if (!ft_strncmp(line[CMD], "cd", 3))
		new_ev = c_cd(line, ev);
	else if (!ft_strncmp(line[CMD], "exit", 5))
		new_ev = c_exit(line, ev);
	else if (!ft_strncmp(line[CMD], "unset", 6))
		new_ev = c_unset(line, ev);
	else if (!ft_strncmp(line[CMD], "export", 7))
		new_ev = c_export(line, ev);
	return (new_ev);
}

int	main(int argc, char **argv, char **ev)
{
	char	**new_ev;
	int		i;

	new_ev = NULL;
	(void)argc;
	new_ev = built_in_check(argv, ev);
	i = 0;
	while (new_ev[i])
	{
		free(new_ev[i]);
		i++;
	}
	free(new_ev);
	return (0);
}
