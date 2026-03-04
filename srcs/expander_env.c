/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:34:08 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:00:49 by shunwata         ###   ########.fr       */
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
	if (!(ft_isalnum(next) || next == '_' || next == '?'
			|| next == '$' || next == '#'))
		return (false);
	if (is_escaped_by_backslash(str, quote, i - 1))
		return (false);
	return (true);
}

static char	*duplicate_env_value(const char *str, int *i, t_mshell *data)
{
	int		start;
	char	*key;
	char	*env_val;

	start = ++(*i);
	if (str[*i] == '?')
		return ((*i)++, ft_itoa(data->exit_status));
	if (str[*i] == '$')
		return ((*i)++, ft_itoa(getpid()));
	if (str[*i] == '#')
		return ((*i)++, ft_itoa(data->ac - 1));
	if (str[*i] == '0')
		return ((*i)++, ft_strdup(data->av[0]));
	if (ft_isdigit(str[*i]))
		return ((*i)++, ft_strdup(""));
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	key = ft_substr(str, start, *i - start);
	if (!key)
		return (NULL);
	env_val = search_get_env(data->ev_clone, key);
	if (!env_val)
		return (free(key), ft_strdup(""));
	return (free(key), ft_strdup(env_val));
}

static void	replace_env_value(char **str, int *i, int start, t_mshell *data)
{
	char	*val;

	val = duplicate_env_value(*str, i, data);
	if (!val || !ft_replace(str, val, start, *i - start))
		(free(val), cleanup(data), exit(1));
	*i = start + ft_strlen(val);
	free(val);
}

void	expand_envs(char **str, t_mshell *data)
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
			replace_env_value(str, &i, start, data);
		}
		else
			i++;
	}
}
