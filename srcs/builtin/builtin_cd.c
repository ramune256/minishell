/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:31 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/26 22:13:46 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// static void	t_pwd_free(t_pwd *pwd)
// {
// 	if (pwd->new_pwd)
// 		free(pwd->new_pwd);
// 	if (pwd->old_pwd)
// 		free(pwd->old_pwd);
// 	if (pwd->new_ev)
// 		free_all(pwd->new_ev);
// }

// static int	add_missing_pwds(t_pwd *pwd, int ev_len)
// {
// 	int	j;

// 	j = ev_len;
// 	if (pwd->f_new_pwd)
// 	{
// 		pwd->new_ev[j] = ft_strjoin("PWD=", pwd->new_pwd);
// 		if (!pwd->new_ev[j])
// 			return(t_pwd_free(pwd),1);
// 		j++;
// 	}
// 	if (pwd->f_old_pwd)
// 	{
// 		pwd->new_ev[j] = ft_strjoin("OLDPWD=", pwd->old_pwd);
// 		if (!pwd->new_ev[j])
// 			return(t_pwd_free(pwd),1);
// 		j++;
// 	}
// 	pwd->new_ev[j] = NULL;
// 	return 0;
// }

// static int	**change_pwd(t_pwd *pwd)
// {
// 	int	i;
// 	int	new_i;

// 	i = 0;
// 	new_i = 0;
// 	while (pwd->ev[i])
// 	{
// 		if (!ft_strncmp(pwd->ev[i], "PWD=", 4))
// 		{
// 			pwd->new_ev[new_i++] = ft_strjoin("PWD=", pwd->new_pwd);
// 			pwd->f_new_pwd = 0;
// 		}
// 		else if (!ft_strncmp(pwd->ev[i], "OLDPWD=", 7))
// 		{
// 			pwd->new_ev[new_i++] = ft_strjoin("OLDPWD=", pwd->old_pwd);
// 			pwd->f_old_pwd = 0;
// 		}
// 		else
// 			pwd->new_ev[new_i++] = ft_strdup(pwd->ev[i]);
// 		if (!pwd->new_ev[new_i - 1])
// 			return(t_pwd_free(pwd),1);
// 		i++;
// 	}
// 	if(add_missing_pwds(pwd, new_i))
// 		return(t_pwd_free(pwd),1);
// 	return (0);
// }

// static int	move_pwd(t_pwd *pwd, char *bash, char *chpath)
// {
// 	pwd->old_pwd = getcwd(NULL, 0);
// 	if (!pwd->old_pwd)
// 		return(t_pwd_free(pwd),1);
// 	if (chdir(chpath) == -1)
// 		return(error(bash, ": cd: No such file or directory\n", NULL),t_pwd_free(pwd),1);
// 	pwd->new_pwd = getcwd(NULL, 0);
// 	if (!pwd->new_pwd)
// 		return(t_pwd_free(pwd),1);
// 	return 0;
// }

// char	**c_cd(char **line, t_alloc *heap)
// {
// 	char	*chpath;
// 	t_pwd	pwd;
// 	int		ev_len;

// 	ev_len = 0;
// 	pwd.f_new_pwd = 1;
// 	pwd.f_old_pwd = 1;
// 	ft_memcpy(&pwd, &(t_pwd){0}, sizeof(t_pwd));
// 	pwd.ev = heap->ev_clone;
// 	while (ev[ev_len])
// 		ev_len++;
// 	if (line[CMD + 2] && line[CMD + 1])
// 		return(error(line[0], ": cd: too many arguments\n", NULL),free(pwd),1);//free(pwd)必要?
// 	if (!line[CMD + 1] || ft_strncmp(line[CMD + 1], "~", 2) == 0)
// 		chpath = serch_get_env(ev, "HOME");
// 	else
// 		chpath = line[CMD + 1];
// 	if(move_pwd(&pwd, line[0], chpath))
// 		return (1);
// 	pwd.new_ev = malloc(sizeof(char *) * (ev_len + 3));
// 	if (!pwd.new_ev)
// 		return(t_pwd_free(&pwd),1);
// 	if(change_pwd(&pwd))
// 		return 1;
// 	heap->ev_clone = pwd.new_ev;
// 	t_pwd_free(pwd);
// 	return (0);
// }

#include "builtin.h"

static void	update_pwd(const char *var, char *pwd, t_alloc *heap)
{
	char	*tmp;

	if (!pwd)
		return ;
	tmp = ft_strjoin(var, pwd);
	free(pwd);
	if (!tmp)
		(cleanup(heap), exit(1));
	update_env(tmp, heap);
	free(tmp);
}

static char	*get_dest_path(char *first_arg, char **ev)
{
	char	*result;

	if (!first_arg) // cdのみ: HOMEへ, cd path: pathへ
	{
		result = search_get_env(ev, "HOME");
		if (!result)
			return (ft_fprintf(stderr, "minishell: cd: HOME not set\n"), NULL);
	}
	else if (ft_strcmp(first_arg, "-") == 0) // ★ cd - の対応
	{
		result = search_get_env(ev, "OLDPWD");
		if (!result)
			return (ft_fprintf(stderr, "minishell: cd: OLDPWD not set\n"), NULL);
		printf("%s\n", result); // bashは移動先を表示する
	}
	else
		result = first_arg;
	return (result);
}

int	c_cd(char **argv, t_alloc *heap)
{
	char	*dest_path;
	char	*oldpwd;
	char	*newpwd;

	if (argv[1] && argv[2])
		return (ft_fprintf(stderr, "minishell: cd: too many arguments\n"), 1);
	dest_path = get_dest_path(argv[1], heap->ev_clone);
	if (!dest_path)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(dest_path) == -1)
		return (perror("minishell: cd"), free(oldpwd), 1);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		ft_fprintf(stderr, "minishell: cd: error retrieving current directory\n");
	update_pwd("OLDPWD=", oldpwd, heap);
	update_pwd("PWD=", newpwd, heap);
	return (0);
}
