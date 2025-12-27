/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:23:06 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 19:14:58 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# define CMD 0
# define OPT 1

# include <limits.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include "minishell.h"

int			c_echo(char **line, t_alloc *heap);
int			c_pwd(char **line, t_alloc *heap);
int			c_env(char **line,t_alloc *heap);
int			c_unset(char **line,t_alloc *heap);
int			c_exit(char **line, t_alloc *heap);
int			c_export(char **line, t_alloc *heap);
int			c_cd(char **line, t_alloc *heap);

long long	ft_atol(const char *st, bool *error);
bool		is_valid_identifier(char *str);
size_t      get_key_len(char *str, int *app_flag);
size_t		get_arr_size(char **arr);
void		sort_str_array(char **arr, size_t size);
int			print_formatted_env(char *env_str);
void		update_env(char *arg, t_alloc *heap);

char		*search_get_env(char **ev, const char *serch);

#endif
