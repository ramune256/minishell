/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2026/02/25 19:58:37 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

static int	print_sorted_env(char **ev_clone, t_alloc *heap)
{
	char	**sorted_ev;
	size_t	size;
	size_t	i;

	if (!ev_clone)
		return (1);
	size = get_arr_size(ev_clone);
	sorted_ev = ft_calloc(size + 1, sizeof(char *));
	if (!sorted_ev)
		(cleanup(heap), exit(1));
	i = 0;
	while (i < size)
	{
		sorted_ev[i] = ev_clone[i];
		i++;
	}
	sorted_ev[i] = NULL;
	sort_str_array(sorted_ev, size);
	i = 0;
	while (i < size)
		if (print_formatted_env(sorted_ev[i++]) < 0)
			return (free(sorted_ev), 1);
	free(sorted_ev);
	return (0);
}

static void	replace_env(char *arg, t_alloc *heap, int i)
{
	char	*tmp;

	tmp = ft_strdup(arg);
	if (!tmp)
		(cleanup(heap), exit(1));
	free(heap->ev_clone[i]);
	heap->ev_clone[i] = tmp;
}

void	update_existing_env(char *arg, t_alloc *heap, int i, bool append_flag)
{
	if (!ft_strchr(arg, '='))
		return ;
	if (append_flag)
		append_env_val(arg, heap, i);
	else
		replace_env(arg, heap, i);
}

void	update_env(char *arg, t_alloc *heap)
{
	size_t	key_len;
	int		i;
	bool	append_flag;
	char	**ev_c;

	ev_c = heap->ev_clone;
	append_flag = false;
	key_len = get_key_len(arg, &append_flag);
	i = 0;
	while (ev_c[i])
	{
		if (!ft_strncmp(ev_c[i], arg, key_len))
		{
			if (ev_c[i][key_len] == '=' || ev_c[i][key_len] == '\0')
			{
				update_existing_env(arg, heap, i, append_flag);
				return ;
			}
		}
		i++;
	}
	append_ev(arg, i, heap);
}

int	c_export(char **argv, t_alloc *heap)
{
	int	i;
	int	status;

	status = 0;
	if (!argv[1])
		return (print_sorted_env(heap->ev_clone, heap));
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			update_env(argv[i], heap);
		i++;
	}
	return (status);
}
