/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 20:40:46 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/25 20:40:51 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# define CMD 0
# define OPT 1

# include <stddef.h>
# include <stdint.h>

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

void		append_ev(char *arg, int count, t_alloc *heap);
void		append_env_val(char *arg, t_alloc *heap, int i);

#endif
