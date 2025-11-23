/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:21:55 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/20 16:31:32 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// `WORD` トークンを解析して EXEC ノードを作成
static t_cmd	*parse_exec_node(t_token **tokens, t_alloc *heap)
{
	t_cmd	*cmd;
	int		argc;
	int		i;

	argc = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		argc++;
		*tokens = (*tokens)->next;
	}
	if (argc == 0) // WORDがなければ EXECノード は作らない
		return (NULL);
	cmd = exec_cmd_constructor();
	if (!cmd)
		(cleanup(heap), exit(1));
	cmd->argv = ft_calloc(argc + 1, sizeof(char *));
	if (!cmd->argv)
		(free_ast(cmd), cleanup(heap), exit(1));
	i = 0;
	while (i < argc)
	{
		cmd->argv[i] = ft_strdup((*tokens)->value);
		if (!cmd->argv[i])
			(free_ast(cmd), cleanup(heap), exit(1));
		*tokens = (*tokens)->next;
		i++;
	}
	cmd->argv[argc] = NULL;
	return (cmd);
}

static t_cmd	*parse_command_unit(t_token **tokens, t_alloc *heap)
{
	t_cmd	*cmd;

	// 1. まずはコマンド本体 (NODE_EXEC) を解析
	// (この時点ではリダイレクトは考慮しない)
	cmd = parse_exec_node(tokens, heap);

	// 2. リダイレクションが続く限り、ループで処理
	while (is_redirection((*tokens)->type))
	{
		// 3. コマンドが空だった場合 (例: `> out`)、
		//    空のEXECノードを作成して `cmd` に設定する
		if (cmd == NULL)
		{
			cmd = exec_cmd_constructor();
			if (!cmd)
				(cleanup(heap), exit(1));
		}
		// 4. `parse_redirection` が `cmd` をラップし、新しい `cmd` を返す
		cmd = parse_redirection(cmd, tokens, heap);
		if (cmd == NULL)
			return (NULL); // エラー（freeはparse_redirection内で行われる）
	}
	// 5. 完成したノード (EXEC または REDIR) を返す
	return (cmd);
}

// パイプライン全体を解析
static t_cmd	*parse_pipeline(t_token **tokens, t_alloc *heap)
{
	t_cmd	*cmd;
	t_cmd	*right;
	t_cmd	*new_pipe_node;

	cmd = parse_command_unit(tokens, heap);
	if (!cmd)
		return (NULL);
	if ((*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->type == TOKEN_EOF || (*tokens)->type == TOKEN_PIPE)
			return (free_ast(cmd), fprintf(stderr, "minishell: syntax error\n"), NULL);
		right = parse_pipeline(tokens, heap);
		if (!right)
			return (free_ast(cmd), NULL);
		new_pipe_node = pipe_cmd_constructor(cmd, right);
		if (!new_pipe_node)
			(free_ast(cmd), free_ast(right), cleanup(heap), exit(1));
		cmd = new_pipe_node;
	}
	return (cmd);
}

// パーサーのエントリーポイント
void	parse(t_alloc *heap)
{
	t_token *tokens;

	tokens = heap->head;
	if (!tokens || tokens->type == TOKEN_EOF)
		return ;
	heap->ast = parse_pipeline(&tokens, heap);
	if (tokens->type != TOKEN_EOF)
	{
		fprintf(stderr, "minishell: syntax error\n");
		free_ast(heap->ast);
		heap->ast = NULL;
	}
}
