/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:23:06 by nmasuda           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/02/09 22:05:57 by shunwata         ###   ########.fr       */
=======
/*   Updated: 2026/02/24 18:41:52 by nmasuda          ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# define CMD 0
# define OPT 1

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
int			c_env(char **line, t_alloc *heap);
int			c_unset(char **line, t_alloc *heap);
int			c_exit(char **line, t_alloc *heap);
int			c_export(char **line, t_alloc *heap);
int			c_cd(char **line, t_alloc *heap);

bool		is_valid_identifier(char *str);
size_t		get_key_len(char *str, bool *append_flag);
size_t		get_arr_size(char **arr);
void		sort_str_array(char **arr, size_t size);
int			print_formatted_env(char *env_str);
void		update_env(char *arg, t_alloc *heap);

#endif
