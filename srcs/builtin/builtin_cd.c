/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:31 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/22 22:25:31 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd(const char *var, char *pwd, t_alloc *heap)
{
	char	*tmp;

	if (!pwd)
		return ;
	tmp = ft_strjoin(var, pwd);
	free(pwd);
	if (!tmp)
		cleanup_and_exit(heap, 1);
	update_env(tmp, heap);
	free(tmp);
}

static char	*get_dest_path(char *first_arg, char **ev)
{
	char	*result;

	if (!first_arg)
	{
		result = search_get_env(ev, "HOME");
		if (!result)
			return (puterr("cd", "HOME not set"), NULL);
	}
	else if (ft_strcmp(first_arg, "-") == 0)
	{
		result = search_get_env(ev, "OLDPWD");
		if (!result)
			return (puterr("cd", "OLDPWD not set"), NULL);
		ft_putstr_fd(result, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
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
		return (puterr("cd", "too many arguments"), 1);
	dest_path = get_dest_path(argv[1], heap->ev_clone);
	if (!dest_path)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(dest_path) == -1)
		return (perror("minishell: cd"), free(oldpwd), 1);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		puterr("cd", "error retrieving current directory");
	update_pwd("OLDPWD=", oldpwd, heap);
	update_pwd("PWD=", newpwd, heap);
	return (0);
}
