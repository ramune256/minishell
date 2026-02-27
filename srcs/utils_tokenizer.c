/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:32:53 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/27 17:53:10 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}

bool	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	skip_spaces(t_lexer *lx)
{
	while (lx->line[lx->index] && ft_strchr(" \t\n", lx->line[lx->index]))
		lx->index++;
}

bool	has_trailing_pipe(t_token *head)
{
	if (!head)
		return (false);
	while (head->next)
		head = head->next;
	if (head->type == TOKEN_PIPE)
		return (true);
	return (false);
}
