/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:07:35 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/20 23:02:49 by shunwata         ###   ########.fr       */
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

t_cmd	*parse_redirection(t_cmd *cmd, t_token **tokens, t_alloc *heap)
{
	t_token	*redir_token;
	char	*filename;
	int		mode;
	int		fd;
	t_cmd	*new_redir_node;

	mode = 0;
	fd = 0;
	redir_token = *tokens;
	*tokens = (*tokens)->next;
	if ((*tokens)->type != TOKEN_WORD)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token", 2);
		return (free_ast(cmd), NULL);
	}
	filename = ft_strdup((*tokens)->value);
	if (!filename)
		(free_ast(cmd), cleanup(heap), rl_clear_history(), exit(1));
	*tokens = (*tokens)->next;
	set_redir_mode_fd(redir_token, &mode, &fd);
	new_redir_node = redir_cmd_constructor(cmd, filename, mode, fd);
	if (!new_redir_node)
		(free_ast(cmd), free(filename), cleanup(heap), rl_clear_history(), exit(1));
	return (new_redir_node);
}
