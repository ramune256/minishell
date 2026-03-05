/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:19:23 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 21:59:28 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_token(t_lexer *lx, t_token_type token_type, char *value)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token || (value == NULL && token_type != TOKEN_EOF))
		(cleanup(lx->data), exit(1));
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
	const char	*p;

	p = lx->line + lx->index;
	if (*p == '|')
		(append_token(lx, TOKEN_PIPE, ft_strdup("|")), lx->index++);
	else if (*p == '<' && *(p + 1) == '<')
		(append_token(lx, TOKEN_HEREDOC, ft_strdup("<<")), lx->index += 2);
	else if (*p == '>' && *(p + 1) == '>')
		(append_token(lx, TOKEN_REDIR_APPEND, ft_strdup(">>")), lx->index += 2);
	else if (*p == '<')
		(append_token(lx, TOKEN_REDIR_IN, ft_strdup("<")), lx->index++);
	else if (*p == '>')
		(append_token(lx, TOKEN_REDIR_OUT, ft_strdup(">")), lx->index++);
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

void	tokenize(t_mshell *data)
{
	t_lexer	lx;

	ft_bzero(&lx, sizeof(t_lexer));
	lx.line = data->line;
	lx.data = data;
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
	append_token(&lx, TOKEN_EOF, NULL);
	data->head = lx.head;
}
