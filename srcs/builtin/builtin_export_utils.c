/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:31:26 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/10 17:59:30 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ev_strlen(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '=')
		i++;
	return (i);
}

char	**change(char **new_ev, int i, int j)
{
	char	*tmp;

	tmp = new_ev[j];
	new_ev[j] = new_ev[i];
	new_ev[i] = tmp;
	return (new_ev);
}

char	**sort(char **new_ev)
{
	int	i;
	int	j;

	i = 0;
	j = i + 1;
	while (new_ev[j] != NULL)
	{
		if (ft_memcmp(new_ev[i], new_ev[j]) > 0)
		{
			new_ev = change(new_ev, i, j);
			i = 0;
			j = i + 1;
		}
		else
			(void)(i++, j++);
	}
	return (new_ev);
}

void	join_error_check(char **new_ev, char **line, int i, int j)
{
	new_ev[j] = ft_export_strjoin("declare -x ", line[i]);
	if (!new_ev[j])
		error(NULL, "export_ft_strjoin_malloc_error\n", new_ev, 2);
}
