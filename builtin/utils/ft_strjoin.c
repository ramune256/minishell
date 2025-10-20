/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:45:32 by nmasuda           #+#    #+#             */
/*   Updated: 2025/10/20 15:31:57 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*box;
	char	*ret;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	box = malloc(len1 + len2 + 1);
	if (!box)
		return (NULL);
	ret = box;
	while (*s1)
		*box++ = *s1++;
	while (*s2)
		*box++ = *s2++;
	*box++ = '\0';
	return (ret);
}
