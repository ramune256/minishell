/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/10 18:22:25 by nmasuda          ###   ########.fr       */
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
			join_error_check(new_ev, ev, k, j++);
		k++;
	}
	return (j);
}

char	**c_export(char **line, char **ev)
{
	char	**new_ev;
	int		i;
	int		j;
	int		ev_cnt;

	ev_cnt = 0;
	while (ev[ev_cnt])
		ev_cnt++;
	new_ev = malloc(sizeof(char *) * (malloc_cnt(line, ev) + 1));
	if (!new_ev)
		error(NULL, "export_newev_malloc_error\n", NULL, 2);
	j = overwrite_ev(ev_cnt, ev, new_ev, line);
	i = CMD + 1;
	while (line[i])
	{
		if (dup_check_for_export(line, i) == true)
			join_error_check(new_ev, line, i, j++);
		i++;
	}
	new_ev[j] = NULL;
	new_ev = sort(new_ev);
	if (!line[CMD + 1])
		c_check(line[0], new_ev);
	return (new_ev);
}
