/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ft_atol.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:39:12 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/22 22:25:24 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_spaces(const char **s)
{
	while (**s == ' ' || (**s >= 9 && **s <= 13))
		(*s)++;
}

static bool	check_overflow(unsigned long long res, int next_digit, int sign)
{
	unsigned long long	cutoff;
	int					cutlim;

	// LLONG_MAX = 9223372036854775807
	cutoff = LLONG_MAX / 10; // 922337203685477580
	cutlim = LLONG_MAX % 10; // 7
	// 負の数の場合、絶対値は LLONG_MIN (9223372036854775808) までOKなので
	// 最後の桁の許容値は 8 になる
	if (sign == -1)
		cutlim += 1;
	// 1. 既にcutoffを超えているなら、次の一桁を足すと確実に溢れる
	if (res > cutoff)
		return (true);
	// 2. cutoffと同じ値なら、最後の桁が許容値を超えていないかチェック
	if (res == cutoff && next_digit > cutlim)
		return (true);
	return (false);
}

long long	set_error_flag(bool *error)
{
	*error = true;
	return (1);
}

long long	ft_atol(const char *str, bool *error)
{
	unsigned long long	res;
	int					sign;

	res = 0;
	sign = 1;
	skip_spaces(&str);
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!(*str >= '0' && *str <= '9'))
		return (set_error_flag(error));
	while (*str >= '0' && *str <= '9')
	{
		if (check_overflow(res, *str - '0', sign))
			return (set_error_flag(error));
		res = res * 10 + (*str - '0');
		str++;
	}
	if (*str != '\0')
		return (set_error_flag(error));
	return ((long long)(res * sign));
}
