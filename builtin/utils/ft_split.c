/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:52:58 by nmasuda           #+#    #+#             */
/*   Updated: 2025/10/20 15:31:57 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

static void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*s1;

	s1 = (unsigned char *)s;
	while (n)
		*s1++ = (n--, c);
	return (s);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	all_size;
	void	*box;

	if (nmemb == 0 || size == 0)
	{
		box = malloc(1);
		if (!box)
			return (NULL);
		box = ft_memset(box, 0, 1);
		return (box);
	}
	if (SIZE_MAX / nmemb < size)
		return (NULL);
	all_size = nmemb * size;
	box = malloc(all_size);
	if (!box)
		return (NULL);
	box = ft_memset(box, '\0', all_size);
	return (box);
}

static size_t	ft_count_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		s++;
		count++;
	}
	return (count);
}

static char	*ft_strndup(const char *s, size_t n)
{
	char	*c;
	char	*ret;

	c = (char *)malloc(n + 1);
	if (!c)
		return (NULL);
	ret = c;
	while (*s && n)
		*c++ = (n--, *s++);
	*c = '\0';
	return (ret);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		len;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	res = ft_calloc(ft_count_words(s, c) + 1, sizeof(char *));
	if (!res)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (!*s)
			break ;
		len = 0;
		while (s[len] && s[len] != c)
			len++;
		res[i] = ft_strndup(s, len);
		if (!res[i])
			return (free_all(res), NULL);
		s += (i++, len);
	}
	return (res);
}
