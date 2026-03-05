/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:08:52 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:02:16 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_tmp_files(t_list **list)
{
	t_list	*current;
	t_list	*tmp;

	current = *list;
	while (current)
	{
		tmp = current->next;
		unlink((char *)current->content);
		free(current->content);
		free(current);
		current = tmp;
	}
	*list = NULL;
}

bool	is_delimiter(const char *line, const char *delimiter)
{
	size_t	len;
	bool	atty;

	atty = isatty(STDIN_FILENO);
	len = ft_strlen(delimiter);
	if (atty && ft_strcmp(line, delimiter) == 0)
		return (true);
	if (!atty && ft_strncmp(line, delimiter, len) == 0 && line[len] == '\n')
		return (true);
	return (false);
}

char	*generate_tmp_filename(t_mshell *data)
{
	static int	id = 0;
	char		*num;
	char		*filename;

	num = ft_itoa(id++);
	if (!num)
		(cleanup(data), exit(1));
	filename = ft_strjoin("/tmp/.minishell_heredoc_", num);
	free(num);
	if (!filename)
		(cleanup(data), exit(1));
	return (filename);
}
