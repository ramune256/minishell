/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:00:39 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/12 18:01:49 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_parent_builtin(t_cmd *ast)
{
	char	*cmd;

	if (ast == NULL || ast->type != NODE_EXEC || ast->argv == NULL)
		return (false);
	cmd = ast->argv[0];
	if (cmd == NULL)
		return (false);
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", 4) == 0)
		return (true);
	if (ft_strncmp(cmd, "export", 6) == 0 && ast->argv[1] != NULL)
		return (true); // 引数ありのexport
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return (true);
	return (false);
}

bool	execute_builtin(t_cmd *exec_node, t_alloc *heap, char **envp)
{
	char	*cmd;
	int		status_code;

	if (exec_node == NULL || exec_node->argv == NULL)
		return (0);
	cmd = exec_node->argv[0];
	if (cmd == NULL)
		return (0);
	status_code = 0; // TODO: 終了ステータスを heap->exit_status に保存

	// if (ft_strncmp(cmd, "echo", 4) == 0)
	// 	status_code = c_echo(exec_node->argv);
	// else if (ft_strncmp(cmd, "cd", 2) == 0)
	// 	status_code = c_cd(exec_node->argv, envp); // (実装によります)
	// else if (ft_strncmp(cmd, "pwd", 3) == 0)
	// 	status_code = c_pwd();
	// else if (ft_strncmp(cmd, "export", 6) == 0)
	// 	status_code = c_export(exec_node->argv, envp); // (実装によります)
	// else if (ft_strncmp(cmd, "unset", 5) == 0)
	// 	status_code = c_unset(exec_node->argv, envp); // (実装によります)
	// else if (ft_strncmp(cmd, "env", 3) == 0)
	// 	status_code = c_env(envp);
	// else if (ft_strncmp(cmd, "exit", 4) == 0)
	// 	c_exit(exec_node->argv, heap); // exitは戻らない
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		status_code = c_pwd(exec_node->argv);
	else
		return (false); // ビルトインではなかった

	// (TODO: status_code を heap->exit_status に保存する処理)
	return (true); // ビルトインを実行した
}
