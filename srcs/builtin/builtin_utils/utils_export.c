/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:31:26 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 19:26:35 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str[i] || (!ft_isalpha(str[i]) && str[i] != '_'))
		return (false);
	i++;
	while (str[i] && str[i] != '=' && str[i] != '_')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	if (str[i] == '+' && str[i + 1] != '=')
		return (false);
	return (true);
}

size_t	get_key_len(char *str, int *app_flag)
{
	size_t	len;

	len = 0;
	if (app_flag)
		*app_flag = 0;
	while (str[len] && str[len] != '=')
	{
		if (str[len] == '+' && str[len + 1] == '=')
		{
			if(app_flag)
				*app_flag = 1;
			return (len);
		}
		len++;
	}
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
		if (write(1, "declare -x ", 12) < 0)
			return (1);
		key_len = equal_pos - env_str;
		if (write(1, env_str, key_len) < 0)
			return (1);
		if (printf("=\"") < 0)
			return (1);
		if (printf("%s", equal_pos + 1) < 0)
			return (1);
		if (printf("\"\n") < 0)
			return (1);
	}
	else
	{
		if (printf("declare -x %s\n", env_str) < 0)
			return (1);
	}
	return (0);
}
