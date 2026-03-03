/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:32:53 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/03 19:07:54 by nmasuda          ###   ########.fr       */
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
