#include "minishell.h"

static bool is_empty_cmd(t_cmd *cmd)
{
	if (cmd->type == NODE_REDIR)
		return (false);
	if (cmd->type == NODE_EXEC && cmd->argv == NULL)
		return (true);
	return (false);
}

static void	append_an_arg(t_cmd *cmd, char *arg, t_alloc *heap)
{
	char	**new_argv;
	int		size;
	int		i;

	size = 0;
	if (cmd->argv)
	{
		while (cmd->argv[size])
			size++;
	}
	new_argv = ft_calloc(size + 1 + 1, sizeof(char *));
	if (!new_argv)
		(cleanup(heap), rl_clear_history(), exit(1));
	i = 0;
	while (i < size)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(arg);
	if (!new_argv[i])
		(free(new_argv), cleanup(heap), rl_clear_history(), exit(1));
	free(cmd->argv);
	cmd->argv = new_argv;
}

static t_cmd	*parse_command_unit(t_token **tokens, t_alloc *heap)
{
	t_cmd	*node_exec;
	t_cmd	*result_ptr;

	node_exec = exec_cmd_constructor();
	if (!node_exec)
		(cleanup(heap), rl_clear_history(), exit(1));
	result_ptr = node_exec;
	while (*tokens && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
	{
		if (is_redirection((*tokens)->type))
		{
			result_ptr = parse_redirection(result_ptr, tokens, heap);
			if (!result_ptr)
				return (NULL);
		}
		else
		{
			append_an_arg(node_exec, (*tokens)->value, heap);
			*tokens = (*tokens)->next;
		}
	}
	return (result_ptr);
}

static t_cmd	*parse_pipeline(t_token **tokens, t_alloc *heap)
{
	t_cmd	*cmd;
	t_cmd	*right;
	t_cmd	*new_pipe_node;

	cmd = parse_command_unit(tokens, heap);
	if (!cmd)
		 return (ft_putstr_fd("minishell: syntax error\n", 2), NULL);
	if ((*tokens)->type == TOKEN_PIPE)
	{
		if (is_empty_cmd(cmd))
			return (free_ast(cmd), ft_putstr_fd("minishell: syntax error near unexpected token '|'\n", 2), NULL);
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_EOF || (*tokens)->type == TOKEN_PIPE)
			return (free_ast(cmd), ft_putstr_fd("minishell: syntax error\n", 2), NULL);
		right = parse_pipeline(tokens, heap);
		if (!right)
			return (free_ast(cmd), NULL);
		new_pipe_node = pipe_cmd_constructor(cmd, right);
		if (!new_pipe_node)
			(free_ast(cmd), free_ast(right), cleanup(heap), rl_clear_history(), exit(1));
		cmd = new_pipe_node;
	}
	return (cmd);
}

void	parse(t_alloc *heap)
{
	t_token *tokens;

	tokens = heap->head;
	if (!tokens || tokens->type == TOKEN_EOF)
		return ;
	heap->ast = parse_pipeline(&tokens, heap);
	if (!heap->ast)
		return ;
	if (tokens->type != TOKEN_EOF)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		free_ast(heap->ast);
		heap->ast = NULL;
	}
}
