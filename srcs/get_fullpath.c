/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fullpath.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:01 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/11 21:10:04 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_envp_path(char **envp)
{
	size_t	i;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (envp[i] + 5);
}

static char	*join_path(char *bin_dir, char *cmd_name)
{
	size_t	total_len;
	char	*fullpath;

	total_len = ft_strlen(bin_dir) + ft_strlen(cmd_name) + 1;
	fullpath = malloc(sizeof(char) * (total_len + 1));
	if (!fullpath)
		return (NULL);
	ft_strlcpy(fullpath, bin_dir, total_len + 1);
	ft_strlcat(fullpath, "/", total_len + 1);
	ft_strlcat(fullpath, cmd_name, total_len + 1);
	return (fullpath);
}

static char	*check_path_and_perm(char **bin_dir, char **cmd_args, t_get_fpath_err *get_fpath_err)
{
	char	*fullpath;
	size_t	i;

	i = 0;
	while (bin_dir[i])
	{
		fullpath = join_path(bin_dir[i], cmd_args[0]);
		if (!fullpath)
			return (free_2d_array(bin_dir), free_2d_array(cmd_args), get_fpath_err = MALLOC_FAILED, (NULL));
		if (access(fullpath, F_OK) == 0)
		{
			if (access(fullpath, X_OK) == 0)
				return (fullpath);
			get_fpath_err = PERM_DENIED;
		}
		free(fullpath);
		i++;
	}
	return (NULL);
}

static char	*check_absolute_path(char **cmd_args, t_get_fpath_err *get_fpath_err)
{
	if (ft_strchr(cmd_args[0], '/'))
	{
		if (access(cmd_args[0], F_OK) != 0)
			return (get_fpath_err = NO_SUCH_FORD, NULL);
		if (access(cmd_args[0], X_OK) != 0)
			return (get_fpath_err = PERM_DENIED, NULL);;
		return (ft_strdup(cmd_args[0]));
	}
	return (NULL);
}

char	*get_fullpath(char **cmd_args, char **envp, t_get_fpath_err get_fpath_err)
{
	char				**bin_dir;
	char				*fullpath;
	char				*envp_path;

	get_fpath_err = NO_ERR;
	if (!cmd_args[0])
		return (NULL);
	fullpath = check_absolute_path(cmd_args, &get_fpath_err);
	if (fullpath)
		return (fullpath);
	if (get_fpath_err == NO_SUCH_FORD || get_fpath_err == PERM_DENIED)
		return (NULL);
	envp_path = find_envp_path(envp);
	if (!envp_path)
		return (NULL);
	bin_dir = ft_split(envp_path, ':');
	if (!bin_dir)
		malloc_failed(cmd_args);
	fullpath = check_path_and_perm(bin_dir, cmd_args, &get_fpath_err);
	if (fullpath)
		return (free_2d_array(bin_dir), fullpath);
	return (free_2d_array(bin_dir), NULL);
}
