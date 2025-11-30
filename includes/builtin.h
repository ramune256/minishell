/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:23:06 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/30 17:23:23 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# define CMD 0
# define OPT 1
# define ERROR -1

# include <limits.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include "minishell.h"

// typedef struct s_pwd
// {
// 	char	**ev;
// 	char	**new_ev;
// 	char	*new_pwd;
// 	char	*old_pwd;
// 	int		f_new_pwd;
// 	int		f_old_pwd;
// }			t_pwd;

//-----main---------
// char		**built_in_check(char **line, char **ev);

//-----command------
int		c_echo(char **line, t_alloc *heap);
int		c_pwd(char **line, t_alloc *heap);
int		c_env(char **line,t_alloc *heap);
int		c_unset(char **line,t_alloc *heap);
int		c_exit(char **line, t_alloc *heap);
int		c_export(char **line, t_alloc *heap);
int		c_cd(char **line, t_alloc *heap);

//-----仮置系--------
// void		error(char *bash, char *mess, char **line, int exit_num);
// void		c_check(char *bash, char **check);
// void		free_all(char **res);

//-----utils---------
long long	ft_atol(const char *st, bool *error);
// char		*ft_export_strjoin(char const *s1, char const *s2);
// size_t		ev_strlen(char *line);
// char		**change(char **new_ev, int i, int j);
// char		**sort(char **new_ev);
// bool		join_error_check(char **new_ev, char **line, int i, int j);
bool		is_valid_identifier(char *str);
size_t		get_key_len(char *str);
size_t		get_arr_size(char **arr);
void		sort_str_array(char **arr, size_t size);
int			print_formatted_env(char *env_str);
void		update_env(char *arg, t_alloc *heap);

char		*search_get_env(char **ev, char *serch);

#endif
