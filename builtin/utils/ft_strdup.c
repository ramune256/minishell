/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:57:48 by nmasuda           #+#    #+#             */
/*   Updated: 2025/10/20 15:31:57 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

char	*ft_strdup(const char *s)
{
	char	*mal;
	char	*res;

	mal = (char *)malloc(ft_strlen(s) + 1);
	if (!mal)
		return (NULL);
	res = mal;
	while (*s)
		*mal++ = *s++;
	*mal = '\0';
	return (res);
}
