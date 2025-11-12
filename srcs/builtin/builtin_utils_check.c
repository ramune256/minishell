/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:08:09 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/08 20:47:24 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	c_check(char *bash, char **check)
{
	int	i;

	i = 0;
	if (!check)
		error(bash, NULL, NULL, 0);
	while (check[i])
	{
		printf("%s\n", check[i]);
		i++;
	}
}
