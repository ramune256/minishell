/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_executor2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 21:27:57 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:02:19 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_exec_node(t_cmd *node)
{
	while (node && node->type == NODE_REDIR)
		node = node->subcmd;
	return (node);
}

void	backup_stdio(int backups[2], t_mshell *data)
{
	backups[STDIN_FILENO] = dup(STDIN_FILENO);
	backups[STDOUT_FILENO] = dup(STDOUT_FILENO);
	if (backups[STDIN_FILENO] == -1 || backups[STDOUT_FILENO] == -1)
		(perror("dup"), cleanup(data), exit(1));
}

void	restore_stdio(int backups[2], t_mshell *data)
{
	if (dup2(backups[STDIN_FILENO], STDIN_FILENO) == -1)
		(perror("dup2"), cleanup(data), exit(1));
	if (dup2(backups[STDOUT_FILENO], STDOUT_FILENO) == -1)
		(perror("dup2"), cleanup(data), exit(1));
	close(backups[STDIN_FILENO]);
	close(backups[STDOUT_FILENO]);
}

bool	apply_redirections(t_cmd *node)
{
	int	file_fd;

	if (!node || node->type != NODE_REDIR)
		return (true);
	if (node->subcmd && node->subcmd->type == NODE_REDIR)
	{
		if (apply_redirections(node->subcmd) == false)
			return (false);
	}
	file_fd = open(node->file, node->mode, 0644);
	if (file_fd == -1)
		return (perror(node->file), false);
	if (dup2(file_fd, node->fd) == -1)
		return (perror("dup2"), close(file_fd), false);
	close(file_fd);
	return (true);
}
