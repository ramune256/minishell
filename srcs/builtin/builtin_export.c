/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:43:46 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/24 00:07:39 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	print_sorted_env(char **ev_clone, t_alloc *heap)
{
	char	**sorted_ev;
	size_t	size;
	size_t	i;

	if (!ev_clone)
		return (0);
	size = get_arr_size(ev_clone); // 配列のサイズを取得
	sorted_ev = ft_calloc(size + 1, sizeof(char *)); //中身の文字列は複製せず、ポインタだけコピーする
	if (!sorted_ev)
		(cleanup(heap), exit(1));
	i = 0;
	while (i < size)
		sorted_ev[i] = ev_clone[i++];
	sorted_ev[i] = NULL;
	sort_str_array(sorted_ev, size);
	i = 0;
	while (i < size)
	{
		if (print_formatted_env(sorted_ev[i]) == ERROR)
			return (free(sorted_ev), 1);
		i++;
	}
	free(sorted_ev); // 配列の枠だけ解放 (中身の文字列は ev_clone が持っているので free しない)
	return (0);
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

	key_len = get_key_len(arg);
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
				free(heap->ev_clone[i]);
				heap->ev_clone[i] = new_str;
			}
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
			fprintf(stderr, "minishell: export: '%s': not a valid identifier\n", argv[i]);
			status = 1;
		}
		else
			update_env(argv[i], heap);
		i++;
	}
	return (status);
}

// bool	dup_check(char **line, char **ev, int i)
// {
// 	int	j;

// 	j = 0;
// 	while (ev[j])
// 	{
// 		if (!ft_strncmp(ev[j], line[i], ev_strlen(line[i]) + 1))
// 			return (false);
// 		j++;
// 	}
// 	j = i + 1;
// 	while (line[j])
// 	{
// 		if (!ft_strncmp(line[j], line[i], ev_strlen(line[i]) + 1))
// 			return (false);
// 		j++;
// 	}
// 	return (true);
// }

// size_t	malloc_cnt(char **line, char **ev)
// {
// 	int		i;
// 	int		cnt;
// 	size_t	ev_cnt;

// 	ev_cnt = 0;
// 	while (ev[ev_cnt])
// 		ev_cnt++;
// 	cnt = 0;
// 	i = CMD + 1;
// 	while (line[i])
// 	{
// 		if (dup_check(line, ev, i) == true)
// 			cnt++;
// 		i++;
// 	}
// 	return (ev_cnt + cnt);
// }

// bool	dup_check_for_export(char **line, int i)
// {
// 	int	j;

// 	j = i + 1;
// 	while (line[j])
// 	{
// 		if (!ft_strncmp(line[j], line[i], ev_strlen(line[i]) + 1))
// 			return (false);
// 		j++;
// 	}
// 	return (true);
// }

// int	overwrite_ev(int ev_cnt, char **ev, char **new_ev, char **line)
// {
// 	int	k;
// 	int	i;
// 	int	j;
// 	int	skip;

// 	j = 0;
// 	k = 0;
// 	while (k < ev_cnt)
// 	{
// 		skip = false;
// 		i = CMD + 1;
// 		while (line[i])
// 		{
// 			if (!ft_strncmp(ev[k], line[i], ev_strlen(line[i]) + 1))
// 			{
// 				skip = true;
// 				break ;
// 			}
// 			i++;
// 		}
// 		if (!skip)
// 		{
// 			if(!join_error_check(new_ev, ev, k, j++))
// 				return(all_free(new_ev),-1);
// 		}
// 		k++;
// 	}
// 	return (j);
// }

// int	c_export(char **line, t_alloc *heap)
// {
// 	char	**res_ev;
// 	int		i;
// 	int		j;
// 	int		ev_cnt;

// 	ev_cnt = 0;
// 	while (heap->new_ev[ev_cnt])
// 		ev_cnt++;
// 	res_ev = malloc(sizeof(char *) * (malloc_cnt(line, heap->new_ev) + 1));
// 	if (!res_ev)
// 		return (1);
// 	j = overwrite_ev(ev_cnt, heap->new_ev, res_ev, line);
// 	if(j == -1)
// 		return (1);
// 	i = CMD + 1;
// 	while (line[i])
// 	{
// 		if (dup_check_for_export(line, i) == true)
// 		{
// 			if(!join_error_check(res_ev, line, i, j++))
// 				return (all_free(res_ev),1);
// 		}
// 		i++;
// 	}
// 	res_ev[j] = NULL;
// 	res_ev = sort(res_ev);
// 	if (!line[CMD + 1])
// 		c_check(line[0], res_ev);
// 	heap->new_ev = res_ev;
// 	return (0);
// }
