/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:34:08 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/23 17:15:10 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_val(const char *str, int *i, t_alloc *heap)
{
	int		start;
	char	*key;
	char	*env_val;
	char	*result;

	(*i)++; // '$'
	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(heap->exit_status));
	}
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	key = ft_substr(str, start, *i - start); // 変数名の長さが0 (例: "$" だけ) の場合などは呼び出し元で弾く必要あり
	if (!key)
		return (NULL);
	env_val = search_get_env(heap->ev_clone, key);
	free(key);
	if (env_val)
		result = ft_strdup(env_val);
	else
		result = ft_strdup("");
	return (result);
}

static void	expand_envs(char **str, t_alloc *heap)
{
	char	*val;
	int		i;
	int		start;
	char	quote;

	if (!str || !*str)
		return ;
	i = 0;
	quote = 0;
	while ((*str)[i])
	{
		if (!quote && ((*str)[i] == '\'' || (*str)[i] == '\"'))
			quote = (*str)[i];
		else if (quote && (*str)[i] == quote)
			quote = 0;
		if ((*str)[i] == '$' && quote != '\'' && (ft_isalnum((*str)[i + 1]) || (*str)[i + 1] == '_' || (*str)[i + 1] == '?'))
		{
			start = i;
			val = get_env_val(*str, &i, heap);
			if (!val)
				(cleanup(heap), exit(1));
			if(!ft_replace(str, val, start, i - start))
				(free(val), cleanup(heap), exit(1));
			i = start + ft_strlen(val);
			free(val);
		}
		else
			i++;
	}
}

static char	*remove_quotes(const char *str)
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
	return (new_str);
}

static void	process_an_arg(char **arg, t_alloc *heap)
{
	char	*result;

	expand_envs(arg, heap);
	result = remove_quotes(*arg);
	if (!result)
		(cleanup(heap), exit(1));
	free(*arg);
	*arg = result;
}

static void	check_args(t_cmd *ast, t_alloc *heap)
{
	int	i;

	i = 0;
	while (ast->argv[i])
	{
		process_an_arg(&(ast->argv[i]), heap);
		i++;
	}
}

void	expand(t_cmd *ast, t_alloc *heap)
{
	if (ast == NULL)
		return ;
	if (ast->type == NODE_EXEC)
		check_args(ast, heap);
	if (ast->type == NODE_REDIR)
		process_an_arg(&(ast->file), heap);
	if (ast->left)
		expand(ast->left, heap);
	if (ast->right)
		expand(ast->right, heap);
	if (ast->subcmd)
		expand(ast->subcmd, heap);
}
