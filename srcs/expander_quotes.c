/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:34:08 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/22 20:34:08 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	should_strip_backslash(char quote, char next)
{
	if (quote == '\'')
		return (false);
	if (quote == 0)
		return (true);
	if (quote == '\"' && (next == '$' || next == '\"' || next == '\\'))
		return (true);
	return (false);
}

static bool	append_escaped_char(
	const char *src, char *dst, int index[2], char quote)
{
	if (src[index[0]] != '\\'
		|| !should_strip_backslash(quote, src[index[0] + 1]))
		return (false);
	index[0]++;
	if (src[index[0]])
	{
		dst[index[1]] = src[index[0]];
		index[1]++;
		index[0]++;
	}
	return (true);
}

static bool	update_quote_state(const char *str, int index[2], char *quote)
{
	if (!*quote && (str[index[0]] == '\'' || str[index[0]] == '\"'))
	{
		*quote = str[index[0]];
		index[0]++;
		return (true);
	}
	if (*quote && str[index[0]] == *quote)
	{
		*quote = 0;
		index[0]++;
		return (true);
	}
	return (false);
}

static void	append_plain_char(const char *src, char *dst, int index[2])
{
	dst[index[1]] = src[index[0]];
	index[1]++;
	index[0]++;
}

char	*remove_quotes(const char *str)
{
	char	*new_str;
	int		index[2];
	char	quote;

	if (!str)
		return (NULL);
	new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	index[0] = 0;
	index[1] = 0;
	quote = 0;
	while (str[index[0]])
	{
		if (update_quote_state(str, index, &quote))
			continue ;
		if (!append_escaped_char(str, new_str, index, quote))
			append_plain_char(str, new_str, index);
	}
	return (new_str);
}
