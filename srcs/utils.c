/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:18 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/27 20:49:22 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	free_2d_array(char **array)
// {
// 	size_t	i;

// 	if (!array)
// 		return ;
// 	i = 0;
// 	while (array[i])
// 	{
// 		free(array[i]);
// 		i++;
// 	}
// 	free(array);
// }

size_t	ft_strnlen(const char *s, size_t n)
{
	const char	*h = s;

	while (n && *s)
		(void)(n--, s++);
	return (s - h);
}

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	size;
	char	*result;

	size = ft_strnlen(s1, n) + 1;
	result = (char *)malloc(size);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, size);
	return (result);
}

void	cleanup(t_alloc *alloc)
{
	free_ast(alloc->ast);
	free_tokens(alloc->head);
	free(alloc->line);
}
