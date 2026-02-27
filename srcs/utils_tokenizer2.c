/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:51:21 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/27 17:53:11 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	join_lines(t_alloc *heap, char *new_line)
{
	char	*tmp;
	char	*joined;

	if (isatty(STDIN_FILENO))
	{
		tmp = ft_strjoin(heap->line, "\n");
		if (!tmp)
			(free(new_line), cleanup(heap), exit(1));
		joined = ft_strjoin(tmp, new_line);
		free(tmp);
	}
	else
		joined = ft_strjoin(heap->line, new_line);
	if (!joined)
		(free(new_line), cleanup(heap), exit(1));
	free(heap->line);
	heap->line = joined;
}

bool	append_input(t_alloc *heap)
{
	char	*new_line;

	new_line = NULL;
	get_input(&new_line, "> ");
	if (new_line == NULL)
	{
		heap->exit_status = 2;
		return (false);
	}
	join_lines(heap, new_line);
	free(new_line);
	return (true);
}

void	request_missing_quote(t_lexer *lx)
{
	if (!append_input(lx->heap))
	{
		free_tokens(lx->head);
		lx->heap->head = NULL;
		return ;
	}
	free_tokens(lx->head);
	lx->heap->head = NULL;
	tokenize(lx->heap);
}

void	request_missing_pipe(t_lexer *lx)
{
	if (!append_input(lx->heap))
	{
		lx->heap->exit_status = 2;
		free_tokens(lx->head);
		lx->head = NULL;
		lx->heap->head = NULL;
		ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
		return ;
	}
	free_tokens(lx->head);
	lx->heap->head = NULL;
	tokenize(lx->heap);
}
