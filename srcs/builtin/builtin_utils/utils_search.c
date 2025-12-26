/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_search.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:15:32 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/23 16:00:26 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*search_get_env(char **ev, const char *search)
{
	int		i;
	size_t	len;

	if (!search)
		return (NULL);
	i = 0;
	len = ft_strlen(search);
	while (ev[i])
	{
		if (!ft_strncmp(ev[i], search, len))
		{
			if (ev[i][len] == '=')
				return (ev[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}
