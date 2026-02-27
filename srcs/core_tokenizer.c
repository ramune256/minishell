/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:19:23 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/27 15:35:11 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_lexer
{
	const char	*line;
	int			index;
	t_token		*head;
	t_token		*tail;
	t_alloc		*heap;
}	t_lexer;

static void	append_token(t_lexer *lx, t_token_type token_type, char *value)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token || (value == NULL && token_type != TOKEN_EOF))
		(cleanup(lx->heap), exit(1));
	new_token->type = token_type;
	new_token->value = value;
	new_token->next = NULL;
	if (!lx->head)
	{
		lx->head = new_token;
		lx->tail = new_token;
		return ;
	}
	lx->tail->next = new_token;
	lx->tail = new_token;
}

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

static bool	append_input(t_alloc *heap)
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

static bool	scan_quotes(const char *line, int *i)
{
	char	quote;

	quote = 0;
	while (line[*i])
	{
		if (quote == 0 && (line[*i] == '\'' || line[*i] == '"'))
			quote = line[*i];
		else if (quote != 0 && line[*i] == quote)
			quote = 0;
		if (quote == 0 && (ft_strchr(" \t\n", line[*i])
				|| is_metachar(line[*i])))
			break ;
		(*i)++;
	}
	if (quote != 0)
		return (false);
	return (true);
}

static void	read_operator(t_lexer *lx)
{
	if (lx->line[lx->index] == '|')
		(append_token(lx, TOKEN_PIPE,
			ft_strndup(lx->line + lx->index, 1)), lx->index++);
	else if (lx->line[lx->index] == '<' && lx->line[lx->index + 1] == '<')
		(append_token(lx, TOKEN_HEREDOC,
			ft_strndup(lx->line + lx->index, 2)), lx->index += 2);
	else if (lx->line[lx->index] == '>' && lx->line[lx->index + 1] == '>')
		(append_token(lx, TOKEN_REDIR_APPEND,
			ft_strndup(lx->line + lx->index, 2)), lx->index += 2);
	else if (lx->line[lx->index] == '<')
		(append_token(lx, TOKEN_REDIR_IN,
			ft_strndup(lx->line + lx->index, 1)), lx->index++);
	else if (lx->line[lx->index] == '>')
		(append_token(lx, TOKEN_REDIR_OUT,
			ft_strndup(lx->line + lx->index, 1)), lx->index++);
}

static bool	scan_word(t_lexer *lx)
{
	int	start;
	int	i;

	start = lx->index;
	i = lx->index;
	if (!scan_quotes(lx->line, &i))
		return (false);
	append_token(lx, TOKEN_WORD, ft_strndup(lx->line + start, i - start));
	lx->index = i;
	return (true);
}

static void	skip_spaces(t_lexer *lx)
{
	while (lx->line[lx->index] && ft_strchr(" \t\n", lx->line[lx->index]))
		lx->index++;
}

static bool	has_trailing_pipe(t_token *head)
{
	if (!head)
		return (false);
	while (head->next)
		head = head->next;
	if (head->type == TOKEN_PIPE)
		return (true);
	return (false);
}

static void	request_missing_quote(t_lexer *lx)
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

static void	request_missing_pipe(t_lexer *lx)
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

void	tokenize(t_alloc *heap)
{
	t_lexer	lx;

	ft_bzero(&lx, sizeof(t_lexer));
	lx.line = heap->line;
	lx.heap = heap;
	while (lx.line[lx.index])
	{
		skip_spaces(&lx);
		if (!lx.line[lx.index])
			break ;
		if (is_metachar(lx.line[lx.index]))
			read_operator(&lx);
		else if (!scan_word(&lx))
		{
			request_missing_quote(&lx);
			return ;
		}
	}
	if (has_trailing_pipe(lx.head))
	{
		request_missing_pipe(&lx);
		return ;
	}
	append_token(&lx, TOKEN_EOF, NULL);
	heap->head = lx.head;
}
