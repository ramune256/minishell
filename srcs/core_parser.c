/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 21:55:21 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 21:59:21 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define ERR_SYNTAX "minishell: syntax error\n"
#define ERR_NOCMD "minishell: syntax error near unexpected token '|'\n"

static void	append_an_arg(t_cmd *cmd, char *arg, t_mshell *data)
{
	char	**new_argv;
	int		size;
	int		i;

	size = 0;
	if (!arg)
		arg = " ";
	if (cmd->argv)
		while (cmd->argv[size])
			size++;
	new_argv = ft_calloc(size + 1 + 1, sizeof(char *));
	if (!new_argv)
		(cleanup(data), exit(1));
	i = 0;
	while (i < size)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(arg);
	if (!new_argv[i])
		(free(new_argv), cleanup(data), exit(1));
	free(cmd->argv);
	cmd->argv = new_argv;
}

static t_cmd	*parse_command_unit(t_token **tokens, t_mshell *data)
{
	t_cmd			*node_exec;
	t_cmd			*result_ptr;

	node_exec = exec_cmd_constructor();
	if (!node_exec)
		(cleanup(data), exit(1));
	result_ptr = node_exec;
	while (!is_end_cmd(*tokens))
	{
		if (is_redirection((*tokens)->type))
		{
			result_ptr = parse_redirection(result_ptr, tokens, data);
			if (!result_ptr)
				return (NULL);
		}
		else
		{
			append_an_arg(node_exec, (*tokens)->value, data);
			*tokens = (*tokens)->next;
		}
	}
	return (result_ptr);
}

static t_cmd	*parse_pipeline(t_token **tokens, t_mshell *data)
{
	t_cmd	*cmd;
	t_cmd	*right;
	t_cmd	*new_pipe_node;

	cmd = parse_command_unit(tokens, data);
	if (!cmd)
		return (ft_putstr_fd(ERR_SYNTAX, 2), NULL);
	if ((*tokens)->type == TOKEN_PIPE)
	{
		if (is_empty_cmd(cmd))
			return (free_ast(cmd), ft_putstr_fd(ERR_NOCMD, 2), NULL);
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_EOF || (*tokens)->type == TOKEN_PIPE)
			return (free_ast(cmd), ft_putstr_fd(ERR_NOCMD, 2), NULL);
		right = parse_pipeline(tokens, data);
		if (!right)
			return (free_ast(cmd), NULL);
		new_pipe_node = pipe_cmd_constructor(cmd, right);
		if (!new_pipe_node)
			(free_ast(cmd), free_ast(right), cleanup(data), exit(1));
		cmd = new_pipe_node;
	}
	return (cmd);
}

void	parse(t_mshell *data)
{
	t_token	*tokens;

	tokens = data->head;
	if (!tokens || tokens->type == TOKEN_EOF)
		return ;
	data->node = parse_pipeline(&tokens, data);
	if (!data->node)
	{
		data->exit_status = 2;
		return ;
	}
	if (tokens->type != TOKEN_EOF)
	{
		free_ast(data->node);
		data->node = NULL;
	}
}
