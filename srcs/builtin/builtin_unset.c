/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:46:10 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/17 04:40:54 by nmasuda          ###   ########.fr       */
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

void  input_new_ev(char **line, char **ev, char **new_ev)
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
		{
//			all_free(new_ev); freeしていいのかわからない
			return 1;
		}
		new_ev[j - i] = new_line;
		j++;
	}
	new_ev[j - i] = NULL;
	return 0;
}

char	**c_unset(char **line, t_alloc *heap)
{
	char	**res_ev;
	int		j;
	int		i;

	res_ev = NULL;
	i = 0;
	j = 0;
	while (heap->new_ev[j])
	{
		if (unset_arg_skip(line, heap->new_ev, j) == true)
			i++;
		j++;
	}
	res_ev = malloc(sizeof(char *) * (j - i + 1));
	if (!res_ev)
		return(1);
	return (input_new_ev(line, heap->new_ev, res_ev));
}
