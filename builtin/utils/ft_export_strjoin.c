/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_strjoin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:50:13 by nmasuda           #+#    #+#             */
/*   Updated: 2025/10/20 15:31:57 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

char	*ft_export_strjoin(char const *s1, char const *s2)
{
	char	*box;
	char	*ret;
	int		first;

	first = 1;
	box = malloc(ft_strlen(s1) + ft_strlen(s2) + 3);
	if (!box)
		return (NULL);
	ret = box;
	while (*s1)
		*box++ = *s1++;
	while (*s2)
	{
		*box++ = *s2++;
		if (*s2 == '=' && first == 1)
		{
			*box++ = *s2++;
			*box++ = '"';
			first = 0;
		}
	}
	*box++ = '"';
	*box++ = '\0';
	return (ret);
}
