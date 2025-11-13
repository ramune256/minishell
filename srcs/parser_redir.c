/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:07:35 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/12 22:52:31 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirection(t_token_type type)
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
}

t_cmd	*parse_redirection(t_cmd *cmd, t_token **tokens, t_alloc *heap)
{
	t_token	*redir_token;
	char	*filename;
	int		mode;
	int		fd;
	t_cmd	*new_redir_node;

	// 1. リダイレクトトークン(>, <, <<, >>)を保存・消費
	redir_token = *tokens;
	*tokens = (*tokens)->next;

	// 2. 次がファイル名でなければ構文エラー
	if ((*tokens)->type != TOKEN_WORD)
		return (fprintf(stderr, "minishell: syntax error near unexpected token\n"), free_ast(cmd), NULL); // 渡されたノードを解放する責任を持つ

	// 3. ファイル名（または区切り文字）を複製・消費
	filename = ft_strdup((*tokens)->value);
	if (!filename)
		(free_ast(cmd), cleanup(heap), exit(1));
	*tokens = (*tokens)->next;

	// 4. mode と fd を設定
	if (redir_token->type == TOKEN_HEREDOC)
	{
		mode = TOKEN_HEREDOC;
		fd = STDIN_FILENO;
	}
	else
		set_redir_mode_fd(redir_token, &mode, &fd);

	// 5. 新しいリダイレクトノードを作成して、古いcmdをラップする
	new_redir_node = redir_cmd_constructor(cmd, filename, mode, fd);
	if (!new_redir_node)
		(free_ast(cmd), free(filename), cleanup(heap), exit(1));

	return (new_redir_node);
}
