/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 19:21:18 by nmasuda          ###   ########.fr       */
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
	size = get_arr_size(ev_clone); // 配列のサイズを取得
	sorted_ev = ft_calloc(size + 1, sizeof(char *)); //中身の文字列は複製せず、ポインタだけコピーする
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
	free(sorted_ev); // 配列の枠だけ解放 (中身の文字列は ev_clone が持っているので free しない)
	return (0);
}

static void	append_ev(char *arg, int count, t_alloc *heap, int app_flag)
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
	if (app_flag)
	{
		int cnt = 0;
		while(arg[cnt] != '+')
			cnt++;
		char *tmp = NULL;
		tmp = ft_strjoin(tmp, ft_strchr(arg, '='));
		free(tmp);
	}
	else
		new_ev[i] = ft_strdup(arg);
	new_ev[i] = ft_strdup(arg);
	if (!new_ev[i])
		(free(new_ev), cleanup(heap), exit(1));
	new_ev[i + 1] = NULL;
	free(heap->ev_clone);
	heap->ev_clone = new_ev;
}

void	update_env(char *arg, t_alloc *heap)
{
	char	*new_str;
	size_t	key_len;
	int		i;
	int		app_flag;

	key_len = get_key_len(arg, &app_flag);
	i = 0;
	while (heap->ev_clone[i])
	{
		if (ft_strncmp(heap->ev_clone[i], arg, key_len) == 0 &&
			(heap->ev_clone[i][key_len] == '=' || heap->ev_clone[i][key_len] == '\0'))
		{
			if (ft_strchr(arg, '='))
			{
				new_str = ft_strdup(arg);
				if (!new_str)
					(cleanup(heap), exit(1));
				if(app_flag)
				{
					char *tmp = NULL;
					while(*new_str++ != '=')
						new_str++;
					tmp = ft_strjoin(heap->ev_clone[i],new_str);
					if(!tmp)
						(cleanup(heap), exit(1));
					free(heap->ev_clone[i]);
					heap->ev_clone[i] = tmp;
				}
				else
				{
					free(heap->ev_clone[i]);
					heap->ev_clone[i] = new_str;
				}
				free(heap->ev_clone[i]);
				heap->ev_clone[i] = new_str;
			}
			return ;
		}
		i++;
	}
	append_ev(arg, i, heap, app_flag);
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
			fprintf(stderr, "minishell: export: '%s': not a valid identifier\n", argv[i]); //ft_fprintf
			status = 1;
		}
		else
			update_env(argv[i], heap);
		i++;
	}
	return (status);
}
