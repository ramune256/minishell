/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fullpath.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:01 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/22 00:40:07 by nmasuda          ###   ########.fr       */
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
			(free_2d_array(&bin_dir), cleanup(heap), exit(1));
		if (access(fullpath, F_OK) == 0)
		{
			if (access(fullpath, X_OK) == 0)
				return (fullpath);
			heap->exit_status = 126;
			return (ft_perror(cmd_name, ": Permission denied"), NULL);
		}
		free(fullpath);
		i++;
	}
	heap->exit_status = 127;
	return (ft_perror(cmd_name, ": command not found"), NULL);
}

static char	*check_absolute_path(char *tentative_path, t_alloc *heap)
{
	char	*result;
	struct	stat path_stat;

	if (access(tentative_path, F_OK) != 0)
	{
		heap->exit_status = 127;
		return (ft_perror(tentative_path, ": No such file or directory"), NULL);
	}
	stat(tentative_path, &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		heap->exit_status = 126;
		return (ft_perror(tentative_path, ": Is a directory"), NULL);
	}
	if (access(tentative_path, X_OK) != 0)
	{
		heap->exit_status = 126;
		return (ft_perror(tentative_path, ": Permission denied"), NULL);
	}

	result = ft_strdup(tentative_path);
	if (!result)
		(cleanup(heap), exit(1));
	return (result);
}

char	*get_fullpath(char *cmd_name, t_alloc *heap)
{
	char				**bin_dir;
	char				*fullpath;
	char				*envp_path;

	if (!cmd_name)
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
	{
		fullpath = check_absolute_path(cmd_name, heap);
		return (fullpath);
	}
	envp_path = find_envp_path(heap->ev_clone);
	if (!envp_path)
		return (ft_perror(cmd_name, ": No such file or directory"), NULL);
	bin_dir = ft_split(envp_path, ':');
	if (!bin_dir)
		(cleanup(heap), exit(1));
	fullpath = check_path_and_perm(bin_dir, cmd_name, heap);
	return (free_2d_array(&bin_dir), fullpath);
}
