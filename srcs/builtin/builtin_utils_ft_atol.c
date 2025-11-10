/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:39:12 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/10 17:50:51 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static bool	ft_overflow(long long res, char *st, int neg)
{
	if (neg == 1)
	{
		if ((LLONG_MAX / 10 < res) || (LLONG_MAX / 10 == res && 7 < *st - 0x30))
			return (false);
	}
	else
	{
		if ((LLONG_MAX / 10 < res) || (LLONG_MAX / 10 == res && 8 < *st - 0x30))
			return (false);
	}
	return (true);
}

long long	ft_atol(char *st, int *error)
{
	int					neg;
	unsigned long long	res;

	neg = 1;
	res = 0;
	while (*st == '0')
		st++;
	if (*st == '+' || *st == '-')
	{
		if (*st == '-')
			neg = -1;
		st++;
	}
	while ('0' <= *st && *st <= '9')
	{
		if (ft_overflow(res, st, neg) == false)
			return (*error = 1);
		res = res * 10 + (*st - '0');
		st++;
	}
	if (!(('0' <= *st && *st <= '9') || *st == '\0'))
		return (*error = 1);
	return (neg * res);
}
