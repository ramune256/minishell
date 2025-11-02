/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:21:55 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/02 16:20:00 by shunwata         ###   ########.fr       */
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
	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
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

static bool	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
			type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

static void	set_redir_mode_fd(t_token *redir_token, int *mode, int *fd)
{
	if (redir_token->type == TOKEN_REDIR_OUT) // >
	{
		*mode = O_WRONLY | O_CREAT | O_TRUNC;
		*fd = STDOUT_FILENO;
	}
	else if (redir_token->type == TOKEN_REDIR_APPEND) // >>
	{
		*mode = O_WRONLY | O_CREAT | O_APPEND;
		*fd = STDOUT_FILENO;
	}
	else if (redir_token->type == TOKEN_REDIR_IN) // <
	{
		*mode = O_RDONLY;
		*fd = STDIN_FILENO;
	}
	// (ヒアドキュメント(<<)の処理は、executor側で別途実装が必要です)
}

static t_cmd	*parse_command(t_token **tokens, t_alloc *heap)
{
	t_cmd	*cmd;
	t_token	*redir_token;
	char	*filename;
	int		mode;
	int		fd;

	// 1. まずはコマンド本体 (NODE_EXEC) を解析
	cmd = parse_simple_command(tokens, heap);
	// 2. リダイレクションが続く限り、ループで処理
	// (注: `cmd > out < in` のように、コマンドの後にリダイレクトが続く構文のみ対応)
	while (is_redirection((*tokens)->type))
	{
		// ヒアドキュメント(<<)は特別な処理が必要なので、ここではスキップ
		if ((*tokens)->type == TOKEN_HEREDOC)
		{
			// TODO: ヒアドキュメントの処理を実装
			// (例: `parse_heredoc(tokens)`)
			// ここでは実装を簡単にするため、2トークン進めます
			*tokens = (*tokens)->next; // "<<" を消費
			*tokens = (*tokens)->next; // "DELIMITER" を消費
			continue; // 次のループへ
		}
		// 3. リダイレクト情報を保存
		redir_token = *tokens;
		*tokens = (*tokens)->next; // リダイレクトトークン (>, <, >>) を消費
		// 4. 次がファイル名でなければ構文エラー
		if ((*tokens)->type != TOKEN_WORD)
		{
			fprintf(stderr, "minishell: syntax error near unexpected token\n");
			return (free_ast(cmd), NULL);
		}
		// 5. mode と fd を設定
		set_redir_mode_fd(redir_token, &mode, &fd);
		filename = ft_strdup((*tokens)->value);
		// TODO: filenameのstrdup失敗チェック
		// 6. トークンを消費し、コマンドを「ラップ」する
		*tokens = (*tokens)->next; // ファイル名トークンを消費
		cmd = redir_cmd_constructor(cmd, filename, mode, fd);
	}
	// 7. 完成したノード (EXEC または REDIR) を返す
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
