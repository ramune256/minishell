/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:00:39 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:00:56 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

bool	is_parent_builtin(t_cmd *node)
{
	char	*cmd;

	if (node == NULL || node->type != NODE_EXEC || node->argv == NULL)
		return (false);
	cmd = node->argv[0];
	if (cmd == NULL)
		return (false);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0 && node->argv[1] != NULL)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	return (false);
}

void	execute_parent_builtin(t_cmd *node, t_mshell *data)
{
	int		backups[2];

	backup_stdio(backups, data);
	if (apply_redirections(node) == false)
	{
		restore_stdio(backups, data);
		data->exit_status = 1;
		cleanup_tmp_files(&data->tmp_files);
		return ;
	}
	execute_builtin(get_exec_node(node), data);
	restore_stdio(backups, data);
	cleanup_tmp_files(&data->tmp_files);
}

bool	execute_builtin(t_cmd *exec_node, t_mshell *data)
{
	char	*cmd;

	if (exec_node == NULL || exec_node->argv == NULL)
		return (false);
	cmd = exec_node->argv[0];
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "cd") == 0)
		data->exit_status = c_cd(exec_node->argv, data);
	else if (ft_strcmp(cmd, "echo") == 0)
		data->exit_status = c_echo(exec_node->argv, data);
	else if (ft_strcmp(cmd, "env") == 0)
		data->exit_status = c_env(exec_node->argv, data);
	else if (ft_strcmp(cmd, "exit") == 0)
		data->exit_status = c_exit(exec_node->argv, data);
	else if (ft_strcmp(cmd, "export") == 0)
		data->exit_status = c_export(exec_node->argv, data);
	else if (ft_strcmp(cmd, "pwd") == 0)
		data->exit_status = c_pwd(exec_node->argv, data);
	else if (ft_strcmp(cmd, "unset") == 0)
		data->exit_status = c_unset(exec_node->argv, data);
	else
		return (false);
	return (true);
}
