/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fullpath.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:01 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/31 20:05:49 by shunwata         ###   ########.fr       */
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

static char	*check_path_and_perm(char **bin_dir, char *cmd_name, t_alloc *heap)
{
	char	*fullpath;
	size_t	i;

	i = 0;
	while (bin_dir[i])
	{
		fullpath = join_path(bin_dir[i], cmd_name);
		if (!fullpath)
			(free_2d_array(bin_dir), cleanup(heap), exit(1));
		if (access(fullpath, F_OK) == 0)
		{
			if (access(fullpath, X_OK) == 0)
				return (fullpath);
			return (perror(cmd_name), NULL);
		}
		free(fullpath);
		i++;
	}
	return (NULL);
}

static char	*check_absolute_path(char *tentative_path, t_alloc *heap)
{
	char	*result;

	if (access(tentative_path, X_OK) != 0)
		return (perror(tentative_path), NULL);
	result = ft_strdup(tentative_path);
	if (!result)
		(cleanup(heap), exit(1));
	return (result);
}

char	*get_fullpath(char *cmd_name, char **envp, t_alloc *heap)
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
	envp_path = find_envp_path(envp);
	if (!envp_path)
		return (NULL); //←?
	bin_dir = ft_split(envp_path, ':');
	if (!bin_dir)
		(cleanup(heap), exit(1));
	fullpath = check_path_and_perm(bin_dir, cmd_name, heap);
	return (free_2d_array(bin_dir), fullpath);
}
