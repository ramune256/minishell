/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:19:23 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/29 00:33:12 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//新しいトークンを作って連結リストの後ろに追加
static void	append_token(t_alloc *heap, t_token_type token_type, char *value)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token || (value == NULL && token_type != TOKEN_EOF))
		(cleanup(heap), exit(1));
	new_token->type = token_type;
	new_token->value = value;
	new_token->next = NULL;
	if (!heap->head)
	{
		heap->head = new_token;
		return ;
	}
	tmp = heap->head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
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

static bool	process_quotes(char *line, int *i)
{
	char	quote;

	quote = 0;
	while (line[*i])
	{
		if (quote == 0 && (line[*i] == '\'' || line[*i] == '\"'))
			quote = line[*i];
		else if (quote != 0 && line[*i] == quote)
			quote = 0;
		if (quote == 0 && (ft_strchr(" \t\n", line[*i]) || is_metachar(line[*i])))
			break ;
		(*i)++;
	}
	if (quote != 0)
		return (false);
	return (true);
}

void	tokenize(t_alloc *heap)
{
	int		i;
	int		start;
	char	*line;

	i = 0;
	line = heap->line;
	while (line[i])
	{
		while (line[i] && ft_strchr(" \t\n", line[i])) //空白・タブ・改行を読み飛ばす
			i++;
		if (!line[i])
			break;
		if (line[i] == '|')
			(append_token(heap, TOKEN_PIPE, ft_strndup(line + i, 1)), i++);
		else if (line[i] == '<' && line[i + 1] == '<')
			(append_token(heap, TOKEN_HEREDOC, ft_strndup(line + i, 2)), i += 2);
		else if (line[i] == '>' && line[i + 1] == '>')
			(append_token(heap, TOKEN_REDIR_APPEND, ft_strndup(line + i, 2)), i+=2);
		else if (line[i] == '<')
			(append_token(heap, TOKEN_REDIR_IN, ft_strndup(line + i, 1)), i++);
		else if (line[i] == '>')
			(append_token(heap, TOKEN_REDIR_OUT, ft_strndup(line + i, 1)), i++);
		else
		{
			start = i;
			if (!process_quotes(line, &i))
			{
				if (!append_input(heap))
				{
					free_tokens(heap->head);
					heap->head = NULL;
					return ;
				}
				free_tokens(heap->head);
				heap->head = NULL;
				tokenize(heap);
				return ;
			}
			append_token(heap, TOKEN_WORD, ft_strndup(line + start, i - start));
		}
	}
	append_token(heap, TOKEN_EOF, NULL);
}

// // トークンリストの末尾に新しいトークンを追加するヘルパー
// static void	append_token(t_token **head, t_token *new_token)
// {
// 	if (!*head)
// 	{
// 		*head = new_token;
// 		return ;
// 	}
// 	t_token *current = *head;
// 	while (current->next)
// 		current = current->next;
// 	current->next = new_token;
// }

// // 新しいトークンを作成するヘルパー
// static t_token *new_token(t_token_type type, char *value)
// {
// 	t_token *token = (t_token *)malloc(sizeof(t_token));
// 	token->type = type;
// 	token->value = value;
// 	token->next = NULL;
// 	return (token);
// }
