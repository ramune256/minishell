/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fullpath.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:01 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/28 01:45:35 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_path(char *bin_dir, char *cmd_name)
{
	size_t	total_len;
	char	*fullpath;

	total_len = ft_strlen(bin_dir) + ft_strlen(cmd_name) + 1;
	fullpath = ft_calloc(total_len + 1, sizeof(char));
	if (!fullpath)
		return (NULL);
	ft_strlcpy(fullpath, bin_dir, total_len + 1);
	ft_strlcat(fullpath, "/", total_len + 1);
	ft_strlcat(fullpath, cmd_name, total_len + 1);
	return (fullpath);
}

static char	*check_path_and_perm(char **bin_dir, char *cmd_name, t_alloc *heap)
{
	char	*fullpath;
	size_t	i;

	i = 0;
	while (bin_dir[i])
	{
		fullpath = join_path(bin_dir[i], cmd_name);
		if (!fullpath)
			(free_2d_array(&bin_dir), cleanup_and_exit(heap, 1));
		if (access(fullpath, F_OK) == 0)
		{
			if (access(fullpath, X_OK) == 0)
				return (fullpath);
			puterr(cmd_name, "Permission denied");
			return (free(fullpath), NULL);
		}
		free(fullpath);
		i++;
	}
	return (puterr(cmd_name, "command not found"), NULL);
}

static char	*check_absolute_path(char *tentative_path, t_alloc *heap)
{
	char		*result;
	struct stat	path_stat;

	if (access(tentative_path, F_OK) != 0)
		return (puterr(tentative_path, "No such file or directory"), NULL);
	stat(tentative_path, &path_stat);
	if (S_ISDIR(path_stat.st_mode))
		return (puterr(tentative_path, "Is a directory"), NULL);
	if (access(tentative_path, X_OK) != 0)
		return (puterr(tentative_path, "Permission denied"), NULL);
	result = ft_strdup(tentative_path);
	if (!result)
		cleanup_and_exit(heap, 1);
	return (result);
}

char	*get_fullpath(char *cmd_name, t_alloc *heap)
{
	char	**bin_dir;
	char	*fullpath;
	char	*envp_path;

	if (!cmd_name)
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
		return (check_absolute_path(cmd_name, heap));
	envp_path = search_get_env(heap->ev_clone, "PATH");
	if (!envp_path || ft_strlen(envp_path) == 0)
		return (puterr(cmd_name, "No such file or directory"), NULL);
	bin_dir = ft_split(envp_path, ':');
	if (!bin_dir)
		cleanup_and_exit(heap, 1);
	fullpath = check_path_and_perm(bin_dir, cmd_name, heap);
	return (free_2d_array(&bin_dir), fullpath);
}
