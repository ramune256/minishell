/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:23:06 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/10 18:24:58 by nmasuda          ###   ########.fr       */
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

typedef struct s_pwd
{
	char	**ev;
	char	**new_ev;
	char	*new_pwd;
	char	*old_pwd;
	int		f_new_pwd;
	int		f_old_pwd;
}			t_pwd;

//-----main---------
char		**built_in_check(char **line, char **ev);

//-----command------
char		**c_echo(char **line, char **ev);
char		**c_pwd(char **line, char **ev);
char		**c_env(char **line, char **ev);
char		**c_unset(char **line, char **ev);
char		**c_exit(char **line, char **ev);
char		**c_export(char **line, char **ev);
char		**c_cd(char **line, char **ev);

//-----仮置系--------
void		error(char *bash, char *mess, char **line, int exit_num);
void		c_check(char *bash, char **check);
void		free_all(char **res);

//-----utils---------
long long	ft_atol(char *st, int *error);
char		*ft_export_strjoin(char const *s1, char const *s2);
int			ev_strlen(char *line);
char		**change(char **new_ev, int i, int j);
char		**sort(char **new_ev);
void		join_error_check(char **new_ev, char **line, int i, int j);
char		*serch_get_env(char **ev, char *serch);

#endif
