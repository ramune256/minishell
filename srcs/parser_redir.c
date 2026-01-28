/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:07:35 by shunwata          #+#    #+#             */
/*   Updated: 2026/01/27 02:05:13 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_redir_mode_fd(t_token *redir_token, int *mode, int *fd)
{
	if (redir_token->type == TOKEN_REDIR_OUT)
	{
		*mode = O_WRONLY | O_CREAT | O_TRUNC;
		*fd = STDOUT_FILENO;
	}
	else if (redir_token->type == TOKEN_REDIR_APPEND)
	{
		*mode = O_WRONLY | O_CREAT | O_APPEND;
		*fd = STDOUT_FILENO;
	}
	else if (redir_token->type == TOKEN_REDIR_IN)
	{
		*mode = O_RDONLY;
		*fd = STDIN_FILENO;
	}
	else if (redir_token->type == TOKEN_HEREDOC)
	{
		*mode = TOKEN_HEREDOC;
		*fd = STDIN_FILENO;
	}
}

t_cmd    *parse_redirection(t_cmd *cmd, t_token **tokens, t_alloc *heap)
{
    t_token    *redir_token;
    char    *filename;
    int        mode;
    int        fd;
    t_cmd    *new_redir_node;

    mode = 0;
    fd = 0;
    redir_token = *tokens;
    *tokens = (*tokens)->next;
    if ((*tokens)->type != TOKEN_WORD)
        return (free_ast(cmd), NULL);
    filename = ft_strdup((*tokens)->value);
    if (!filename)
        (free_ast(cmd), cleanup(heap), exit(1));
    *tokens = (*tokens)->next;
    set_redir_mode_fd(redir_token, &mode, &fd);
    new_redir_node = redir_cmd_constructor(cmd, filename, mode, fd);
    if (!new_redir_node)
        (free_ast(cmd), free(filename), cleanup(heap), exit(1));
    return (new_redir_node);
}