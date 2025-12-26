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
			return (fprintf(stderr, "minishell: cd: HOME not set\n"), NULL); //ft_fprintf
	}
	else if (ft_strcmp(first_arg, "-") == 0) // ★ cd - の対応
	{
		result = search_get_env(ev, "OLDPWD");
		if (!result)
			return (fprintf(stderr, "minishell: cd: OLDPWD not set\n"), NULL); //ft_fprintf
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
		return (fprintf(stderr, "minishell: cd: too many arguments\n"), 1); //ft_fprintf
	dest_path = get_dest_path(argv[1], heap->ev_clone);
	if (!dest_path)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(dest_path) == -1)
		return (perror("minishell: cd"), free(oldpwd), 1);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		fprintf(stderr, "minishell: cd: error retrieving current directory\n"); //ft_fprintf
	update_pwd("OLDPWD=", oldpwd, heap);
	update_pwd("PWD=", newpwd, heap);
	return (0);
}
