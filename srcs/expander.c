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

static void	expand_argument(char **arg, t_alloc *heap)
{
	char	*result;

	expand_envs(arg, heap);
	result = remove_quotes(*arg);
	if (!result)
		(cleanup(heap), exit(1));
	free(*arg);
	*arg = result;
}

static void	expand_arguments(t_cmd *node, t_alloc *heap)
{
	int	i;

	i = 0;
	while (node->argv[i])
	{
		expand_argument(&(node->argv[i]), heap);
		i++;
	}
}

void	expand(t_cmd *node, t_alloc *heap)
{
	if (node == NULL)
		return ;
	if (node->type == NODE_EXEC && node->argv)
		expand_arguments(node, heap);
	if (node->type == NODE_REDIR && node->file)
		expand_argument(&(node->file), heap);
	if (node->left)
		expand(node->left, heap);
	if (node->right)
		expand(node->right, heap);
	if (node->subcmd)
		expand(node->subcmd, heap);
}
