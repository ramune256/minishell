/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:31:26 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/20 16:02:31 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

bool	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str[i] || (!ft_isalpha(str[i]) && str[i] != '_'))
		return (false);
	i++;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

size_t	get_key_len(char *str)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	return (len);
}

size_t	get_arr_size(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

void	sort_str_array(char **arr, size_t size)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1 - i)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	print_formatted_env(char *env_str)
{
	char	*equal_pos;
	size_t	key_len;

	equal_pos = ft_strchr(env_str, '=');
	if (equal_pos)
	{
		if (printf("declare -x ") == ERROR)
			return (1);
		key_len = equal_pos - env_str;
		if (write(1, env_str, key_len) == ERROR)
			return (1);
		if (printf("=\"") == ERROR)
			return (1);
		if (printf("%s", equal_pos + 1) == ERROR)
			return (1);
		if (printf("\"\n") == ERROR)
			return (1);
	}
	else
	{
		if (printf("declare -x %s\n", env_str) == ERROR)
			return (1);
	}
	return (0);
}

// char	**sort(char **new_ev)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = i + 1;
// 	while (new_ev[j] != NULL)
// 	{
// 		if (export_memcmp(new_ev[i], new_ev[j]) > 0)
// 		{
// 			new_ev = change(new_ev, i, j);
// 			i = 0;
// 			j = i + 1;
// 		}
// 		else
// 			(void)(i++, j++);
// 	}
// 	return (new_ev);
// }

// size_t	ev_strlen(char *line)
// {
// 	size_t	i;

// 	i = 0;
// 	while (line[i] != '=')
// 		i++;
// 	return (i);
// }

// char	**change(char **new_ev, int i, int j)
// {
// 	char	*tmp;

// 	tmp = new_ev[j];
// 	new_ev[j] = new_ev[i];
// 	new_ev[i] = tmp;
// 	return (new_ev);
// }

// static int	export_memcmp(char *s1, char *s2)
// {
// 	int i;
// 	i = 0;

// 	while (s1[i] && s2[i])
// 	{
// 		if (s1[i] != s2[i])
// 			return (s1[i] - s2[i]);
// 		if(s1[i + 1] == '=' || s2[i + 1] == '=')
// 			return (s1[i] - s2[i]);
// 		i++;
// 	}
// 	return (s1[i] - s2[i]);
// }

// bool	join_error_check(char **new_ev, char **line, int i, int j)
// {
// 	new_ev[j] = ft_export_strjoin("declare -x ", line[i]);
// 	if (!new_ev[j])
// 		return (false);
// 	return (true);
// }
