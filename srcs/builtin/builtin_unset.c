/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:46:10 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/10 18:05:20 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static bool	unset_arg_skip(char **line, char **ev, int j)
{
	int		i;
	size_t	len;

	i = 1;
	len = ft_strlen(line[CMD + i]);
	while (line[CMD + i])
	{
		if (!ft_strncmp(ev[j], line[CMD + i], len))
		{
			if (ev[j][len] == '=' || ev[j][len] == '\0')
				return (true);
		}
		else
			i++;
	}
	return (false);
}

char	**input_new_ev(char **line, char **ev, char **new_ev)
{
	int		i;
	int		j;
	char	*new_line;

	i = 0;
	j = 0;
	new_line = NULL;
	while (ev[j])
	{
		if (unset_arg_skip(line, ev, j) == true)
		{
			(void)(j++, i++);
			if (!ev[j])
				break ;
		}
		if (!ft_strncmp(ev[j], "_=", 2))
			break ;
		new_line = ft_strdup(ev[j]);
		if (!new_line)
			error(NULL, "unset_newline_malloc_error\n", new_ev, 2);
		new_ev[j - i] = new_line;
		j++;
	}
	new_ev[j - i] = NULL;
	return (new_ev);
}

char	**c_unset(char **line, char **ev)
{
	char	**new_ev;
	int		j;
	int		i;

	new_ev = NULL;
	i = 0;
	j = 0;
	while (ev[j])
	{
		if (unset_arg_skip(line, ev, j) == true)
			i++;
		j++;
	}
	new_ev = malloc(sizeof(char *) * (j - i + 1));
	if (!new_ev)
		error(NULL, "unset_newev_malloc_error\n", NULL, 2);
	return (input_new_ev(line, ev, new_ev));
}
