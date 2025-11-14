/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_serch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:15:32 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/14 19:54:27 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

char	*serch_get_env(char **ev, char *serch)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(serch);
	while (ev[i])
	{
		if (!ft_strncmp(ev[i], serch, len))
		{
			if (ev[i][len] == '=')
				return (ev[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}
