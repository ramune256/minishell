/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 17:14:57 by nmasuda           #+#    #+#             */
/*   Updated: 2025/10/20 15:31:57 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

static void	double_free(char *old_pwd, char *new_pwd)
{
	free(old_pwd);
	free(new_pwd);
}

static char	**change_pwd(char *old_pwd, char *new_pwd, char **ev)
{
	char	**new_ev;
	int		i;

	i = 0;
	while (ev[i])
		i++;
	new_ev = malloc(sizeof(char *) * (i + 1));
	if (!new_ev)
		error(NULL, "cd_malloc_error\n", NULL, 1);
	i = 0;
	while (ev[i])
	{
		if (!ft_strncmp(ev[i], "PWD=", 4))
			new_ev[i] = ft_strjoin("PWD=", new_pwd);
		else if (!ft_strncmp(ev[i], "OLDPWD=", 7))
			new_ev[i] = ft_strjoin("OLDPWD=", old_pwd);
		else
			new_ev[i] = ft_strdup(ev[i]);
		if (!new_ev[i])
			error(NULL, "cd_getpwd_error\n", new_ev, 1);
		i++;
	}
	new_ev[i] = NULL;
	double_free(new_pwd, old_pwd);
	return (new_ev);
}

char	**c_cd(char **line, char **ev)
{
	char	*chpath;
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = NULL;
	new_pwd = NULL;
	if (line[CMD + 2] && line[CMD + 1])
		error(line[0], ": cd: too many arguments\n", NULL, 1);
	if (line[CMD + 1] == NULL || ft_strncmp(line[CMD + 1], "~", 2) == 0)
		chpath = getenv("HOME");
	else
		chpath = line[CMD + 1];
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		error(NULL, "cd_getpwd_error\n", NULL, 1);
	if (chdir(chpath) == -1)
	{
		free(old_pwd);
		error(line[0], ": cd: /home/uea: No such file or directory\n", NULL, 1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		error(NULL, "cd_getpwd_error\n", NULL, 1);
	return (change_pwd(old_pwd, new_pwd, ev));
}
