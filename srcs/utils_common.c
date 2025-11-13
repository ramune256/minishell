/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_common.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:18 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/12 22:26:31 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	cleanup_temp_files(&alloc->temp_files);
	ft_bzero(alloc, sizeof(t_alloc));
}

void	get_input(char **line, const char *message)
{
	if (isatty(STDIN_FILENO))
		*line = readline(message);
	else
		*line = get_next_line(STDIN_FILENO);
}

void	print_exit(void)
{
	if (isatty(STDIN_FILENO))
		printf("exit\n");
}

//isatty 1  readline
//       0  gnl 

void	free_2d_array(char **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
