/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:31:26 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 22:53:56 by shunwata         ###   ########.fr       */
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
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (true);
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

size_t	get_key_len(char *str, bool *append_flag)
{
	size_t	len;

	len = 0;
	if (append_flag)
		*append_flag = 0;
	while (str[len] && str[len] != '=')
	{
		if (str[len] == '+' && str[len + 1] == '=')
		{
			if (append_flag)
				*append_flag = 1;
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

	equal_pos = ft_strchr(env_str, '=');
	if (equal_pos)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		write(STDOUT_FILENO, env_str, equal_pos - env_str);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(equal_pos + 1, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env_str, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (0);
}
