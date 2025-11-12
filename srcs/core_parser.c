/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:21:55 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/12 18:10:26 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// `WORD` トークンを解析して EXEC ノードを作成
static t_cmd	*parse_simple_command(t_token **tokens, t_alloc *heap)
{
	t_cmd	*cmd;
	int		argc;
	t_token *start;
	int		i;

	argc = 0;
	start = *tokens;
	while (start && start->type == TOKEN_WORD)
	{
		argc++;
		start = start->next;
	}
	if (argc == 0)
		return (NULL);
	cmd = exec_cmd_constructor();
	cmd->argv = ft_calloc(1, sizeof(char *) * (argc + 1));
	if (!cmd->argv)
		(cleanup(heap), exit(1));
	i = 0;
	while (i < argc)
	{
		cmd->argv[i] = ft_strdup((*tokens)->value);
		if (!cmd->argv[i])
			(cleanup(heap), exit(1));
		*tokens = (*tokens)->next;
		i++;
	}
	cmd->argv[argc] = NULL;
	return (cmd);
}

static t_cmd	*parse_command(t_token **tokens, t_alloc *heap)
{
	t_cmd	*cmd;
	t_token	*redir_token;
	char	*filename;
	int		mode;
	int		fd;

	cmd = parse_simple_command(tokens, heap); // 1. まずはコマンド本体 (NODE_EXEC) を解析
	if (!cmd) // コマンドが空の場合（例: `> out` のみ）
		cmd = exec_cmd_constructor(); // 空のEXECノードを作成
	// 2. リダイレクションが続く限り、ループで処理
	while (is_redirection((*tokens)->type))
	{
		// 3. リダイレクトトークン(>, <, <<, >>)を保存・消費
		redir_token = *tokens;
		*tokens = (*tokens)->next;
		// 4. 次がファイル名（または区切り文字）でなければ構文エラー
		if ((*tokens)->type != TOKEN_WORD)
			return (fprintf(stderr, "minishell: syntax error near unexpected token\n"), free_ast(cmd), NULL);
		// 5. ファイル名（または区切り文字）を複製
		filename = ft_strdup((*tokens)->value);
		if (!filename)
			(cleanup(heap), exit(1)); // malloc失敗
		// 6. ファイル名トークンを消費
		*tokens = (*tokens)->next;
		// 7. ヒアドキュメントかどうかで処理を分岐
		if (redir_token->type == TOKEN_HEREDOC)
		{
			mode = TOKEN_HEREDOC; // executorが見分けるための「目印」
			fd = STDIN_FILENO;   // ヒアドキュメントは標準入力(0)
		}
		else
			set_redir_mode_fd(redir_token, &mode, &fd);
		// 8. コマンドをリダイレクトノードで「ラップ」する
		cmd = redir_cmd_constructor(cmd, filename, mode, fd);
		if (!cmd)
			(free_ast(cmd), cleanup(heap), exit(1)); // malloc失敗
	}
	// 9. 完成したノード (EXEC または REDIR) を返す
	return (cmd);
}

// パイプライン全体を解析
static t_cmd	*parse_pipeline(t_token **tokens, t_alloc *heap)
{
	t_cmd	*cmd;
	t_cmd	*right;

	cmd = parse_command(tokens, heap);
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
		cmd = pipe_cmd_constructor(cmd, right);
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
