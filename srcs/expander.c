/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:34:08 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/27 18:01:31 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_var_head(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

char	*get_env_val(const char *str, int *i, t_alloc *heap)
{
	int		start;
	char	*key;
	char	*env_val;
	char	*result;

	(*i)++;
	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(heap->exit_status));
	}
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	key = ft_substr(str, start, *i - start);
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

static bool	check_escape(const char *str, char quote, int i, size_t count)
{
	if (count % 2)
	{
		if (quote == '\'')
			return (false);
		else if (quote == '\"')
		{
			if (str[i + 1] == '$' || str[i + 1] == '\"'
				|| str[i + 1] == '\\')
				return (true);
			return (false);
		}
		else
			return (true);
	}
	else
		return (false);
}

static bool	skip_backslash(const char *str, char quote, int i)
{
	size_t	count;
	size_t	tmp_i;

	count = 0;
	if (i < 0 || str[i] != '\\')
		return (false);
	tmp_i = i;
	while (i >= 0 && str[i] == '\\')
		(void)((i--, count++));
	i = tmp_i;
	return (check_escape(str, quote, i, count));
}

static bool	is_expandable(const char *str, int i, char quote)
{
	if (str[i] != '$')
		return (false);
	if (quote == '\'')
		return (false);
	if (!is_valid_var_head(str[i + 1]))
		return (false);
	if (skip_backslash(str, quote, i - 1))
		return (false);
	return (true);
}

static bool	should_strip_backslash(char quote, char next)
{
	if (quote == '\'')
		return (false);
	if (quote == 0)
		return (true);
	if (quote == '\"')
	{
		if (next == '$' || next == '\"' || next == '\\')
			return (true);
	}
	return (false);
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
		if (is_expandable(*str, i, quote))
		{
			start = i;
			val = get_env_val(*str, &i, heap);
			if (!val)
				(cleanup(heap), rl_clear_history(), exit(1));
			if (!ft_replace(str, val, start, i - start))
				(free(val), cleanup(heap), rl_clear_history(), exit(1));
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
	new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i++];
		else if (quote && str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else if (str[i] == '\\' && should_strip_backslash(quote, str[i + 1]))
		{
			i++;
			if (str[i])
				new_str[j++] = str[i++];
		}
		else
			new_str[j++] = str[i++];
	}
	return (new_str);
}

static void	process_an_arg(char **arg, t_alloc *heap)
{
	char	*result;

	expand_envs(arg, heap);
	result = remove_quotes(*arg);
	if (!result)
		(cleanup(heap), rl_clear_history(), exit(1));
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
	if (ast->type == NODE_EXEC && ast->argv)
		check_args(ast, heap);
	if (ast->type == NODE_REDIR && ast->file)
		process_an_arg(&(ast->file), heap);
	if (ast->left)
		expand(ast->left, heap);
	if (ast->right)
		expand(ast->right, heap);
	if (ast->subcmd)
		expand(ast->subcmd, heap);
}
