/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:00:39 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/28 20:04:51 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

bool	is_parent_builtin(t_cmd *ast)
{
	char	*cmd;

	if (ast == NULL || ast->type != NODE_EXEC || ast->argv == NULL)
		return (false);
	cmd = ast->argv[0];
	if (cmd == NULL)
		return (false);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0 && ast->argv[1] != NULL)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	return (false);
}

bool	execute_builtin(t_cmd *exec_node, t_alloc *heap)
{
	char	*cmd;

	if (exec_node == NULL || exec_node->argv == NULL)
		return (0);
	cmd = exec_node->argv[0];
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		heap->exit_status = c_cd(exec_node->argv, heap);
	else if (ft_strcmp(cmd, "echo") == 0)
		heap->exit_status = c_echo(exec_node->argv, heap);
	else if (ft_strcmp(cmd, "env") == 0)
		heap->exit_status = c_env(exec_node->argv, heap);
	else if (ft_strcmp(cmd, "exit") == 0)
		heap->exit_status = c_exit(exec_node->argv, heap);
	else if (ft_strcmp(cmd, "export") == 0)
		heap->exit_status = c_export(exec_node->argv, heap);
	else if (ft_strcmp(cmd, "pwd") == 0)
		heap->exit_status = c_pwd(exec_node->argv, heap);
	else if (ft_strcmp(cmd, "unset") == 0)
		heap->exit_status = c_unset(exec_node->argv, heap);
	else
		return (false);
	return (true);
}
