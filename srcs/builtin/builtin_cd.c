/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:31 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/17 05:04:44 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	t_pwd_free(t_pwd *pwd)
{
	if (pwd->new_pwd)
		free(pwd->new_pwd);
	if (pwd->old_pwd)
		free(pwd->old_pwd);
	if (pwd->new_ev)
		free_all(pwd->new_ev);
}

static int	add_missing_pwds(t_pwd *pwd, int ev_len)
{
	int	j;

	j = ev_len;
	if (pwd->f_new_pwd)
	{
		pwd->new_ev[j] = ft_strjoin("PWD=", pwd->new_pwd);
		if (!pwd->new_ev[j])
			return(t_pwd_free(pwd),1);
		j++;
	}
	if (pwd->f_old_pwd)
	{
		pwd->new_ev[j] = ft_strjoin("OLDPWD=", pwd->old_pwd);
		if (!pwd->new_ev[j])
			return(t_pwd_free(pwd),1);
		j++;
	}
	pwd->new_ev[j] = NULL;
	return 0;
}

static int	**change_pwd(t_pwd *pwd)
{
	int	i;
	int	new_i;

	i = 0;
	new_i = 0;
	while (pwd->ev[i])
	{
		if (!ft_strncmp(pwd->ev[i], "PWD=", 4))
		{
			pwd->new_ev[new_i++] = ft_strjoin("PWD=", pwd->new_pwd);
			pwd->f_new_pwd = 0;
		}
		else if (!ft_strncmp(pwd->ev[i], "OLDPWD=", 7))
		{
			pwd->new_ev[new_i++] = ft_strjoin("OLDPWD=", pwd->old_pwd);
			pwd->f_old_pwd = 0;
		}
		else
			pwd->new_ev[new_i++] = ft_strdup(pwd->ev[i]);
		if (!pwd->new_ev[new_i - 1])
			return(t_pwd_free(pwd),1);
		i++;
	}
	if(add_missing_pwds(pwd, new_i))
		return(t_pwd_free(pwd),1);
	return (0);
}

static int	move_pwd(t_pwd *pwd, char *bash, char *chpath)
{
	pwd->old_pwd = getcwd(NULL, 0);
	if (!pwd->old_pwd)
		return(t_pwd_free(pwd),1);
	if (chdir(chpath) == -1)
		return(error(bash, ": cd: No such file or directory\n", NULL),t_pwd_free(pwd),1);
	pwd->new_pwd = getcwd(NULL, 0);
	if (!pwd->new_pwd)
		return(t_pwd_free(pwd),1);
	return 0;
}

char	**c_cd(char **line, t_alloc *heap)
{
	char	*chpath;
	t_pwd	pwd;
	int		ev_len;

	ev_len = 0;
	pwd.f_new_pwd = 1;
	pwd.f_old_pwd = 1;
	ft_memcpy(&pwd, &(t_pwd){0}, sizeof(t_pwd));
	pwd.ev = heap->new_ev;
	while (ev[ev_len])
		ev_len++;
	if (line[CMD + 2] && line[CMD + 1])
		return(error(line[0], ": cd: too many arguments\n", NULL),free(pwd),1);//free(pwd)必要?
	if (!line[CMD + 1] || ft_strncmp(line[CMD + 1], "~", 2) == 0)
		chpath = serch_get_env(ev, "HOME");
	else
		chpath = line[CMD + 1];
	if(move_pwd(&pwd, line[0], chpath))
		return (1);
	pwd.new_ev = malloc(sizeof(char *) * (ev_len + 3));
	if (!pwd.new_ev)
		return(t_pwd_free(&pwd),1);
	if(change_pwd(&pwd))
		return 1;
	heap->new_ev = pwd.new_ev;
	t_pwd_free(pwd);
	return (0);
}
