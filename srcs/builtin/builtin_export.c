/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/17 05:11:08 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

bool	dup_check(char **line, char **ev, int i)
{
	int	j;

	j = 0;
	while (ev[j])
	{
		if (!ft_strncmp(ev[j], line[i], ev_strlen(line[i]) + 1))
			return (false);
		j++;
	}
	j = i + 1;
	while (line[j])
	{
		if (!ft_strncmp(line[j], line[i], ev_strlen(line[i]) + 1))
			return (false);
		j++;
	}
	return (true);
}

size_t	malloc_cnt(char **line, char **ev)
{
	int		i;
	int		cnt;
	size_t	ev_cnt;

	ev_cnt = 0;
	while (ev[ev_cnt])
		ev_cnt++;
	cnt = 0;
	i = CMD + 1;
	while (line[i])
	{
		if (dup_check(line, ev, i) == true)
			cnt++;
		i++;
	}
	return (ev_cnt + cnt);
}

bool	dup_check_for_export(char **line, int i)
{
	int	j;

	j = i + 1;
	while (line[j])
	{
		if (!ft_strncmp(line[j], line[i], ev_strlen(line[i]) + 1))
			return (false);
		j++;
	}
	return (true);
}

int	overwrite_ev(int ev_cnt, char **ev, char **new_ev, char **line)
{
	int	k;
	int	i;
	int	j;
	int	skip;

	j = 0;
	k = 0;
	while (k < ev_cnt)
	{
		skip = false;
		i = CMD + 1;
		while (line[i])
		{
			if (!ft_strncmp(ev[k], line[i], ev_strlen(line[i]) + 1))
			{
				skip = true;
				break ;
			}
			i++;
		}
		if (!skip)
		{
			if(join_error_check(new_ev, ev, k, j++))
				return(all_free(new_ev),-1);
		}
		k++;
	}
	return (j);
}

char	**c_export(char **line, t_alloc *heap)
{
	char	**res_ev;
	int		i;
	int		j;
	int		ev_cnt;

	ev_cnt = 0;
	while (heap->new_ev[ev_cnt])
		ev_cnt++;
	res_ev = malloc(sizeof(char *) * (malloc_cnt(line, heap->new_ev) + 1));
	if (!res_ev)
		return (1);
	j = overwrite_ev(ev_cnt, heap->new_ev, res_ev, line);
	if(j == -1)
		return (1);
	i = CMD + 1;
	while (line[i])
	{
		if (dup_check_for_export(line, i) == true)
		{
			if(join_error_check(res_ev, line, i, j++))
				return (all_free(res_ev),1);
		}
		i++;
	}
	res_ev[j] = NULL;
	res_ev = sort(res_ev);
	if (!line[CMD + 1])
		c_check(line[0], res_ev);
	heap->new_ev = res_ev;
	return (0);
}
