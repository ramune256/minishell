/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:39:00 by nmasuda           #+#    #+#             */
/*   Updated: 2025/10/20 15:31:57 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

int	ft_strncmp(char *s1, const char *s2, size_t n)
{
	while (n && *s1 == *s2)
	{
		if (*s1 == '\0')
			return (0);
		(void)(n--, s1++, s2++);
	}
	if (n)
		return ((unsigned char)*s1 - (unsigned char)*s2);
	return (0);
}
