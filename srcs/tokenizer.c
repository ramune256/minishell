/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:19:23 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/11 21:20:09 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// トークンリストの末尾に新しいトークンを追加するヘルパー
static void	append_token(t_token **head, t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	t_token *current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

// 新しいトークンを作成するヘルパー
static t_token *new_token(t_token_type type, char *value)
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

// メタ文字かどうかを判定
static bool is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// NOTE: このTokenizerは簡単化のためクォートを扱いません
t_token	*tokenize(const char *line)
{
	t_token	*head = NULL;
	int		i = 0;

	while (line[i])
	{
		while (line[i] && strchr(" \t\n", line[i]))
			i++;
		if (!line[i])
			break;
		if (line[i] == '|')
		{
			append_token(&head, new_token(TOKEN_PIPE, strndup(line + i, 1)));
			i++;
		}
		else if (line[i] == '<' && line[i + 1] == '<')
		{
			append_token(&head, new_token(TOKEN_HEREDOC, strndup(line + i, 2)));
			i += 2;
		}
		else if (line[i] == '>' && line[i + 1] == '>')
		{
			append_token(&head, new_token(TOKEN_REDIR_APPEND, strndup(line + i, 2)));
			i += 2;
		}
		else if (line[i] == '<')
		{
			append_token(&head, new_token(TOKEN_REDIR_IN, strndup(line + i, 1)));
			i++;
		}
		else if (line[i] == '>')
		{
			append_token(&head, new_token(TOKEN_REDIR_OUT, strndup(line + i, 1)));
			i++;
		}
		else
		{
			int start = i;
			while (line[i] && !strchr(" \t\n", line[i]) && !is_metachar(line[i]))
				i++;
			append_token(&head, new_token(TOKEN_WORD, strndup(line + start, i - start)));
		}
	}
	append_token(&head, new_token(TOKEN_EOF, NULL));
	return (head);
}

void	free_tokens(t_token *token)
{
	t_token *tmp;
	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}
