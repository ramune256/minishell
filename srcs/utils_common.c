/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_common.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:18 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/15 17:30:18 by shunwata         ###   ########.fr       */
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

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	cleanup(t_alloc *alloc)
{
	free(alloc->line);
	free_tokens(alloc->head);
	free_ast(alloc->ast);
	cleanup_temp_files(&alloc->temp_files);
	free_2d_array(alloc->new_ev);
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
