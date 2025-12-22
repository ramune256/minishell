/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:34:08 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/23 01:07:46 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_val(char *str, int *i, t_alloc *heap, int status)
{
	int		start;
	char	*key;
	char	*val;

	(*i)++; // '$'
	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(status));
	}
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	// 変数名の長さが0 (例: "$" だけ) の場合などは呼び出し元で弾く想定
	key = ft_substr(str, start, *i - start);
	if (!key)
		return (NULL);
	val = ft_strdup(search_get_env(heap->ev_clone, key));
	if (!val)
		val = ft_strdup("");
	free(key);
	return (val);
}

static char	*expand_envs(char *str, t_alloc *heap)
{
	char	*val;
	int		i;
	int		start;
	char	quote;

	if (!str)
		return (NULL);
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		if (str[i] == '$' && quote != '\'' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
		{
			start = i;
			val = get_env_val(str, &i, heap, heap->exit_status);
			i += ft_replace(&str, val, start, i - start); //存在しない環境変数はどうなる？
			free(val);
		}
		else
			i++;
	}
	return (str);
}


//ここから上確認！！！


static char	*remove_quotes(char *str)
{
	char	*new_str;
	int		i;
	int		j;
	char	quote;

	if (!str)
		return (NULL);
	new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char)); // 削除後は必ず短くなるので、元の長さで確保
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		else
			new_str[j++] = str[i]; // 中身はコピー
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

static bool	process_an_arg(t_cmd *ast, int i, t_alloc *heap)
{
	char	*expanded;
	char	*result;

	expanded = expand_envs(ast->argv[i], heap);
	result = remove_quotes(expanded);
	free(expanded);
	if (!result)
		(cleanup(heap), exit(1));
	free(ast->argv[i]);
	ast->argv[i] = result;
}

static void	check_args(t_cmd *ast, t_alloc *heap)
{
	int	i;

	i = 0;
	while (ast->argv[i])
	{
		process_arg(ast, i, heap);
		i++;
	}
}

void	expand(t_cmd *ast, t_alloc *heap)
{
	if (ast == NULL)
		return ;
	if (ast->type == NODE_EXEC)
		check_args(ast, heap);
	if (ast->left)
		expand(ast->left, heap);
	if (ast->right)
		expand(ast->right, heap);
	if (ast->subcmd)
		expand(ast->subcmd, heap);
}
