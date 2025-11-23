/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:31 by nmasuda           #+#    #+#             */
/*   Updated: 2025/11/24 00:43:08 by shunwata         ###   ########.fr       */
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

void	update_pwd(const char *var, char *pwd, t_alloc *heap)
{
	char	*tmp;

	if (pwd)
	{
		tmp = ft_strjoin(var, pwd);
		free(pwd);
		if (!tmp)
			(cleanup(heap), exit(1));
		update_env(tmp, heap);
		free(tmp);
	}
	else if (ft_strncmp(var, "PWD=", 4))
		ft_putstr_fd("minishell: cd: error retrieving current directory\n", 2);
	// getcwdが失敗した場合(移動先のディレクトリが削除された場合など) エラーメッセージを出しつつも、chdir自体は成功しているので0を返すことも多い
}

int	c_cd(char **argv, t_alloc *heap)
{
	char	*dest_path;
	char	*oldpwd;

	if (!argv[1]) // cdのみ: HOMEへ, cd path: pathへ
	{
		dest_path = search_get_env(heap->ev_clone, "HOME");
		if (!dest_path)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else
		dest_path = argv[1];
	oldpwd = getcwd(NULL, 0);
	// (getcwd失敗時のハンドリング追加、NULLなら空文字にする等)
	if (chdir(dest_path) == -1)
		return (perror("minishell: cd"), free(oldpwd), 1);
	update_pwd("OLDPWD=", oldpwd, heap);
	update_pwd("PWD=", getcwd(NULL, 0), heap);
	return (0);
}
