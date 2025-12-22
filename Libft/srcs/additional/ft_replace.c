/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:09:37 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/22 22:39:51 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "additional.h"

size_t	ft_replace(char **origin, const char *replace, size_t start, size_t len)
{
	char	*new_str;
	size_t	new_len;

	if (!*origin || !replace)
		return ;
	new_len =  ft_strlen(*origin) - len + ft_strlen(replace);
	new_str = ft_calloc(new_len + 1, sizeof(char));
	if (!new_str)
		return ;
	ft_strlcpy(new_str, *origin, start + 1);
	ft_strlcat(new_str, replace, new_len + 1);
	ft_strlcat(new_str, *origin + start + len, new_len + 1);
	free(*origin);
	*origin = new_str;
	return (ft_strlen(replace));
}
