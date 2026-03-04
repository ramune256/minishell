/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:51:21 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:02:12 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	join_lines(t_mshell *data, char *new_line)
{
	char	*tmp;
	char	*joined;

	if (isatty(STDIN_FILENO))
	{
		tmp = ft_strjoin(data->line, "\n");
		if (!tmp)
			(free(new_line), cleanup(data), exit(1));
		joined = ft_strjoin(tmp, new_line);
		free(tmp);
	}
	else
		joined = ft_strjoin(data->line, new_line);
	if (!joined)
		(free(new_line), cleanup(data), exit(1));
	free(data->line);
	data->line = joined;
}

bool	append_input(t_mshell *data)
{
	char	*new_line;

	new_line = NULL;
	get_input(&new_line, "> ");
	if (new_line == NULL)
	{
		data->exit_status = 2;
		return (false);
	}
	join_lines(data, new_line);
	free(new_line);
	return (true);
}

void	request_missing_quote(t_lexer *lx)
{
	if (!append_input(lx->data))
	{
		free_tokens(lx->head);
		lx->data->head = NULL;
		return ;
	}
	free_tokens(lx->head);
	lx->data->head = NULL;
	tokenize(lx->data);
}
