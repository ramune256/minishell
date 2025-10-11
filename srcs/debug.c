/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:24:57 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/11 21:25:12 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ast(t_cmd *cmd, int level)
{
	if (cmd == NULL)
		return ;
	for (int i = 0; i < level; i++)
		printf("  ");
	if (cmd->type == NODE_EXEC)
	{
		printf("EXEC: ");
		for (int i = 0; cmd->argv[i]; i++)
			printf("[%s] ", cmd->argv[i]);
		printf("\n");
	}
	else if (cmd->type == NODE_PIPE)
	{
		printf("PIPE\n");
		print_ast(cmd->left, level + 1);
		print_ast(cmd->right, level + 1);
	}
	else if (cmd->type == NODE_REDIR)
	{
		printf("REDIR (mode: %d, fd: %d, file: %s)\n", cmd->mode, cmd->fd, cmd->file);
		print_ast(cmd->subcmd, level + 1);
	}
}
