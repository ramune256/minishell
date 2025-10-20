/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 23:13:50 by nmasuda           #+#    #+#             */
/*   Updated: 2025/10/17 21:39:43 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# define CMD 1
# define OPT 2
# define ERROR -1

# include <limits.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

//-----main---------
void		built_in_check(char **line, char **ev);

//-----command------
void		c_echo(char **line);
void		c_pwd(char **line);
void		c_env(char **line, char **ev);
char		**c_unset(char **line, char **ev);
void		c_exit(char **line);
char		**c_export(char **line, char **ev);
char		**c_cd(char **line, char **ev);

//-----仮置系--------
void		error(char *bash, char *mess, char **line, int exit_num);
void		c_check(char *bash, char **check);
void		free_all(char **res);

//-----utils---------
int			ft_strncmp(char *s1, const char *s2, size_t n);
size_t		ft_strlen(const char *str);
long long	ft_atoi(char *st, int *error);
char		*ft_strdup(const char *s);
int			ft_memcmp(const void *s1, const void *s2);
char		*ft_export_strjoin(char const *s1, char const *s2);
char		*ft_strjoin(char const *s1, char const *s2);
void		ft_putstr_fd(char *s, int fd);
char		**ft_split(char const *s, char c);

#endif
