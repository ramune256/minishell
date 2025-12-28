/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/28 20:06:23 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	{
		if (print_formatted_env(sorted_ev[i]) < 0)
			return (free(sorted_ev), 1);
		i++;
	}
	free(sorted_ev);
	return (0);
}

static void	append_new_env(char *arg, t_alloc *heap, char **new_ev, int i)
{
	char	*tmp_val;
	char	*tmp_key;
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos && arg[equal_pos - arg - 1] == '+')
	{
		tmp_key = ft_substr(arg, 0, equal_pos - arg - 1);
		if (!tmp_key)
			(free(new_ev), cleanup(heap), exit(1));
		tmp_val = ft_strdup(equal_pos);
		if (!tmp_val)
			(free(tmp_key), free(new_ev), cleanup(heap), exit(1));
		new_ev[i] = ft_strjoin(tmp_key, tmp_val);
		(free(tmp_key), free(tmp_val));
	}
	else
		new_ev[i] = ft_strdup(arg);
	if (!new_ev[i])
		(free(new_ev), cleanup(heap), exit(1));
	new_ev[i + 1] = NULL;
	free(heap->ev_clone);
	heap->ev_clone = new_ev;
}

static void	append_ev(char *arg, int count, t_alloc *heap)
{
	char	**new_ev;
	int		i;

	new_ev = ft_calloc(count + 2, sizeof(char *));
	if (!new_ev)
		(cleanup(heap), exit(1));
	i = 0;
	while (heap->ev_clone[i])
	{
		new_ev[i] = heap->ev_clone[i];
		i++;
	}
	append_new_env(arg, heap, new_ev, i);
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

static void	append_env_val(char *arg, t_alloc *heap, int i)
{
	char	*new_val;
	char	*tmp;
	char	*joined_key;

	new_val = ft_strdup(ft_strchr(arg, '=') + 1);
	if (!new_val)
		(cleanup(heap), exit(1));
	if (ft_strchr(heap->ev_clone[i], '='))
		tmp = ft_strjoin(heap->ev_clone[i], new_val);
	else
	{
		joined_key = ft_strjoin(heap->ev_clone[i], "=");
		if (!joined_key)
			(free(new_val), cleanup(heap), exit(1));
		tmp = ft_strjoin(joined_key, new_val);
		free(joined_key);
	}
	free(new_val);
	if (!tmp)
		(cleanup(heap), exit(1));
	free(heap->ev_clone[i]);
	heap->ev_clone[i] = tmp;
}

static void	update_existing_env(char *arg, t_alloc *heap, int i,
		bool append_flag)
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

	append_flag = false;
	key_len = get_key_len(arg, &append_flag);
	i = 0;
	while (heap->ev_clone[i])
	{
		if (ft_strncmp(heap->ev_clone[i], arg, key_len) == 0
			&& (heap->ev_clone[i][key_len] == '='
				|| heap->ev_clone[i][key_len] == '\0'))
		{
			update_existing_env(arg, heap, i, append_flag);
			return ;
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
