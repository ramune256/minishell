/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:30:58 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/10 19:47:07 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	put_line_to_tmpfile(char *line,char *delimiter, int tmp_fd)
{
	bool	atty;
	bool	is_delimiter;
	size_t	len;

	atty = isatty(STDIN_FILENO);
	len = ft_strlen(delimiter);
	if (atty)
		is_delimiter = (ft_strcmp(line, delimiter) == 0);
	else
		is_delimiter = (ft_strncmp(line, delimiter, len) == 0 && line[len] == '\n');
	if (is_delimiter)
		return (true);
	ft_putstr_fd(line, tmp_fd);
	if (atty)
		ft_putstr_fd("\n", tmp_fd);
	return (false);
}

void	change_fd(int pipefd[2], int target_fd, int fd_num)
{
	dup2(fd_num, target_fd);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	cleanup_temp_files(t_list **list)
{
	t_list	*current;
	t_list	*tmp;

	current = *list;
	while (current)
	{
		tmp = current->next;
		unlink((char *)current->content); // 一時ファイルを削除
		free(current->content); // strdupしたファイル名を解放
		free(current); // リストのノードを解放
		current = tmp;
	}
	*list = NULL;
}
