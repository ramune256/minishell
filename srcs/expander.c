/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:34:08 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:00:51 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_argument(char **arg, t_mshell *data)
{
	char	*result;

	expand_envs(arg, data);
	result = remove_quotes(*arg);
	if (!result)
		(cleanup(data), exit(1));
	free(*arg);
	*arg = result;
}

static void	expand_arguments(t_cmd *node, t_mshell *data)
{
	int	i;

	i = 0;
	while (node->argv[i])
	{
		expand_argument(&(node->argv[i]), data);
		i++;
	}
}

void	expand(t_cmd *node, t_mshell *data)
{
	if (node == NULL)
		return ;
	if (node->type == NODE_EXEC && node->argv)
		expand_arguments(node, data);
	if (node->type == NODE_REDIR && node->file)
		expand_argument(&(node->file), data);
	if (node->left)
		expand(node->left, data);
	if (node->right)
		expand(node->right, data);
	if (node->subcmd)
		expand(node->subcmd, data);
}
