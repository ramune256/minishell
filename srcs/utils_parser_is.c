/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser_is.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 21:04:09 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/25 21:05:21 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

bool	is_empty_cmd(t_cmd *cmd)
{
	if (cmd->type == NODE_REDIR)
		return (false);
	if (cmd->type == NODE_EXEC && cmd->argv == NULL)
		return (true);
	return (false);
}

bool	is_end_cmd(t_token *tokens)
{
	if (!tokens)
		return (true);
	if (tokens->type == TOKEN_PIPE)
		return (true);
	if (tokens->type == TOKEN_EOF)
		return (true);
	return (false);
}
