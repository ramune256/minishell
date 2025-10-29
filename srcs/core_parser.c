/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:21:55 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/29 16:03:58 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// `WORD` トークンを解析して EXEC ノードを作成
static t_cmd	*parse_simple_command(t_token *tokens)
{
	t_cmd	*cmd;
	int		argc;
	t_token *start;

	argc = 0;
	start = tokens;
	while (start && start->type == TOKEN_WORD)
	{
		argc++;
		start = start->next;
	}
	if (argc == 0)
		return (NULL);
	cmd = exec_cmd_constructor();
	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
	for (int i = 0; i < argc; i++)
	{
		cmd->argv[i] = ft_strdup(tokens->value);
		tokens = tokens->next;
	}
	cmd->argv[argc] = NULL;
	return (cmd);
}

// リダイレクションとコマンドを解析
static t_cmd	*parse_command(t_token *tokens)
{
	t_cmd *cmd;

	// まずは左側にリダイレクションがあるか確認
	// (この実装では簡単化のため、コマンド本体の解析のみ)
	cmd = parse_simple_command(tokens);

	// 次に右側にリダイレクションがあるか確認
	// (実際にはここにリダイレクションを処理するループが必要)

	return (cmd);
}

// パイプライン全体を解析
static t_cmd	*parse_pipeline(t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*right;

	cmd = parse_command(tokens);
	if (!cmd)
		return (NULL);

	if (tokens->type == TOKEN_PIPE)
	{
		tokens = tokens->next;
		if (tokens->type == TOKEN_EOF || tokens->type == TOKEN_PIPE)
			return (fprintf(stderr, "minishell: syntax error\n"), NULL);
		right = parse_pipeline(tokens);
		if (!right)
			return (NULL);
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
	heap->ast = parse_pipeline(tokens);
	if (tokens->type != TOKEN_EOF)
	{
		fprintf(stderr, "minishell: syntax error\n");
		free_ast(heap->ast);
		heap->ast = NULL;
	}
}
