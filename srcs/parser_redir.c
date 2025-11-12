/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:07:35 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/12 18:08:27 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
			type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

void	set_redir_mode_fd(t_token *redir_token, int *mode, int *fd)
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
