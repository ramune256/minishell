/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:34:08 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/22 20:34:08 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_escaped_by_backslash(const char *str, char quote, int before)
{
	size_t	count;
	char	next;

	if (before < 0 || str[before] != '\\')
		return (false);
	next = str[before + 1];
	count = 0;
	while (before >= 0 && str[before] == '\\')
		(void)((before--, count++));
	if (!(count % 2))
		return (false);
	if (quote == '\'')
		return (false);
	if (quote == '\"')
		return (next == '$' || next == '\"' || next == '\\');
	return (true);
}

static bool	is_expandable(const char *str, int i, char quote)
{
	char	next;

	if (str[i] != '$' || quote == '\'')
		return (false);
	next = str[i + 1];
	if (!(ft_isalnum(next) || next == '_' || next == '?'))
		return (false);
	if (is_escaped_by_backslash(str, quote, i - 1))
		return (false);
	return (true);
}

static char	*duplicate_env_value(const char *str, int *i, t_alloc *heap)
{
	int		start;
	char	*key;
	char	*env_val;

	(*i)++;
	start = *i;
	if (str[*i] == '?')
		return ((*i)++, ft_itoa(heap->exit_status));
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	key = ft_substr(str, start, *i - start);
	if (!key)
		return (NULL);
	env_val = search_get_env(heap->ev_clone, key);
	free(key);
	if (env_val)
		return (ft_strdup(env_val));
	return (ft_strdup(""));
}

static void	replace_env_value(char **str, int *i, int start, t_alloc *heap)
{
	char	*val;

	val = duplicate_env_value(*str, i, heap);
	if (!val || !ft_replace(str, val, start, *i - start))
		(free(val), cleanup(heap), exit(1));
	*i = start + ft_strlen(val);
	free(val);
}

void	expand_envs(char **str, t_alloc *heap)
{
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
			replace_env_value(str, &i, start, heap);
		}
		else
			i++;
	}
}
