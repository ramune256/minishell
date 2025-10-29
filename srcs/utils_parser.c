/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:56:32 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/29 14:37:51 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*exec_cmd_constructor(void)
{
	t_cmd	*cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	cmd->type = NODE_EXEC;
	return (cmd);
}

t_cmd	*pipe_cmd_constructor(t_cmd *left, t_cmd *right)
{
	t_cmd	*cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	cmd->type = NODE_PIPE;
	cmd->left = left;
	cmd->right = right;
	return (cmd);
}

t_cmd	*redir_cmd_constructor(t_cmd *subcmd, char *file, int mode, int fd)
{
	t_cmd	*cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	cmd->type = NODE_REDIR;
	cmd->subcmd = subcmd;
	cmd->file = file;
	cmd->mode = mode;
	cmd->fd = fd;
	return (cmd);
}

static void	free_argv(char **argv)
{
	if (!argv)
		return;
	for (int i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
}

void	free_ast(t_cmd *cmd)
{
	if (!cmd)
		return;
	if (cmd->type == NODE_EXEC)
		free_argv(cmd->argv);
	else if (cmd->type == NODE_PIPE)
	{
		free_ast(cmd->left);
		free_ast(cmd->right);
	}
	else if (cmd->type == NODE_REDIR)
	{
		free_ast(cmd->subcmd);
		free(cmd->file);
	}
	free(cmd);
}
