/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:31 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 21:59:17 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

static void	execute_command(t_cmd *node, t_mshell *data)
{
	t_cmd	*exec_node;
	char	*fullpath;
	int		tmp_exit_status;

	set_signal_child();
	if (!apply_redirections(node))
		(cleanup(data), exit(1));
	exec_node = get_exec_node(node);
	if (execute_builtin(exec_node, data))
	{
		tmp_exit_status = data->exit_status;
		(cleanup(data), exit(tmp_exit_status));
	}
	if ((!exec_node->argv))
		(cleanup(data), exit(0));
	fullpath = get_fullpath(exec_node->argv[0], data);
	if (fullpath == NULL)
	{
		tmp_exit_status = data->exit_status;
		(cleanup(data), exit(tmp_exit_status));
	}
	if (execve(fullpath, exec_node->argv, data->ev_clone) == -1)
		(perror(fullpath), free(fullpath), cleanup(data), exit(126));
}

static void	execute_exec(t_cmd *node, t_mshell *data)
{
	pid_t	pid;
	int		status;

	if (g_sig_status)
		return ;
	pid = fork();
	if (pid == -1)
		(perror("fork"), cleanup(data), exit(1));
	if (pid == 0)
		execute_command(node, data);
	set_signal_parent();
	waitpid(pid, &status, 0);
	set_signal_shell();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		ft_putstr_fd("\n", STDOUT_FILENO);
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	get_exit_status(data, status);
	cleanup_tmp_files(&data->tmp_files);
}

static void	execute_pipe(t_cmd *node, t_mshell *data)
{
	int		status;
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (node->type == NODE_EXEC)
	{
		execute_exec(node, data);
		return ;
	}
	if (pipe(pipefd) == -1)
		(perror("pipe"), cleanup(data), exit(1));
	pid_left = exec_subnode(node->left, pipefd, STDOUT_FILENO, data);
	pid_right = exec_subnode(node->right, pipefd, STDIN_FILENO, data);
	(close(pipefd[0]), close(pipefd[1]));
	set_signal_parent();
	(waitpid(pid_left, NULL, 0), waitpid(pid_right, &status, 0));
	set_signal_shell();
	get_exit_status(data, status);
}

void	execute(t_cmd *node, t_mshell *data)
{
	if (node == NULL)
		return ;
	if (node->type == NODE_PIPE)
		execute_pipe(node, data);
	else if (node->type == NODE_REDIR)
	{
		if (is_parent_builtin(get_exec_node(node)))
			execute_parent_builtin(node, data);
		else
			execute_exec(node, data);
	}
	else if (node->type == NODE_EXEC)
	{
		if (is_parent_builtin(node))
			execute_builtin(node, data);
		else
			execute_exec(node, data);
	}
}
