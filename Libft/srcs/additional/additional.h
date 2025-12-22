/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 20:06:46 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/22 22:40:30 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDITIONAL_H
# define ADDITIONAL_H

# include "libft.h"

size_t	ft_replace(char **origin, const char *replace, size_t start, size_t len);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strndup(const char *s1, size_t n);
size_t	ft_strnlen(const char *s, size_t n);

#endif
