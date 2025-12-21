/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_common.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:18 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/18 16:22:40 by shunwata         ###   ########.fr       */
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
	char	**ev;
	bool	success;

	ev = alloc->ev_clone;
	success = alloc->success;
	free(alloc->line);
	free_tokens(alloc->head);
	free_ast(alloc->ast);
	cleanup_temp_files(&alloc->temp_files);
	if (success == false)
		free_2d_array(&(alloc->ev_clone));
	ft_bzero(alloc, sizeof(t_alloc));
	if (success == true)
		alloc->ev_clone = ev;
}

void	get_input(char **line, const char *message)
{
	if (isatty(STDIN_FILENO))
		*line = readline(message);
	else
		*line = get_next_line(STDIN_FILENO);
}

void	print_exit(t_alloc *heap)
{
	if (isatty(STDIN_FILENO))
		fprintf(stderr, "exit\n");
	cleanup(heap);
	exit(0);
}

void	free_2d_array(char ***array)
{
	size_t	i;

	if (!array || !*array)
		return ;
	i = 0;
	while ((*array)[i])
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}
