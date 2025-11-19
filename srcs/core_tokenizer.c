/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:19:23 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/14 07:59:04 by nmasuda          ###   ########.fr       */
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

// メタ文字かどうかを判定
static bool is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

//新しいトークンを作って連結リストの後ろに追加
static void	append_token(t_alloc *heap, t_token_type token_type, char *value)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token || (value == NULL && token_type != TOKEN_EOF))
		(cleanup(heap), exit(1));
	new_token->type = token_type;
	if(token_type == TOKEN_WORD)
		value = test(heap,value);//expand_ev_valueの中のものに変える
	if(!value)
		(cleanup(heap), exit(1));
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

void	tokenize(t_alloc *heap, char *line)//ダブルクォーテーションとシングルクォーテーションを飛ばす処理
{
	int		i;
	int		meta_flag;
	int		start;

	i = 0;
	while (line[i])
	{
		meta_flag = true;
		while (line[i] && ft_strchr(" \t\n", line[i])) //空白・タブ・改行を読み飛ばす
			i++;
		if (!line[i])
			break;
		if(line[i] == "\'")
			single_flag++;
		if(line[i] == "\"")
			double_flag++;
		if(single_flag % 2 || double_flag % 2)
			line = meta_check(line,&i,&meta_flag);
		if(!meta_flag)
		{
			start = i;
			while (line[i] && !ft_strchr(" \t\n", line[i]) && !is_metachar(line[i]))
				i++;
			append_token(heap, TOKEN_WORD, ft_strndup(line + start, i - start));
		}
	}
	append_token(heap, TOKEN_EOF, NULL);
}

char * meta_check(char *line,int *i,int *meta_flag)
{
	meta_flag = true;
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
		meta_flag = false;
	return (line);
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