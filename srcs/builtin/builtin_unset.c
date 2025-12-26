/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:46:10 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 02:14:35 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			i++;
		}
		else
			i++;
	}
	return (false);
}

static int	input_new_ev(char **line, t_alloc *heap, int j, int i)
{
	char	**res_ev;

	res_ev = ft_calloc(j - i + 1, sizeof(char *));
	if (!res_ev)
		(cleanup(heap), exit(1));
	i = 0;
	j = 0;
	while (heap->ev_clone[j])
	{
		if (unset_arg_skip(line, heap->ev_clone, j) == true)
		{
			(void)(j++, i++);
			if (!heap->ev_clone[j])
				break ;
		}
		if (!ft_strncmp(heap->ev_clone[j], "_=", 2))
			break ;
		res_ev[j - i] = ft_strdup(heap->ev_clone[j]);
		if (!res_ev[j - i])
			(free_2d_array(&res_ev), cleanup(heap), exit(1));
		j++;
	}
	res_ev[j - i] = NULL;
	free_2d_array(&(heap->ev_clone));
	heap->ev_clone = res_ev;
	return (0);
}

int	c_unset(char **line, t_alloc *heap)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!line[CMD + 1])
		return (0);
	while (heap->ev_clone[j])
	{
		if (unset_arg_skip(line, heap->ev_clone, j) == true)
			i++;
		j++;
	}
	return (input_new_ev(line, heap, j, i));
}
