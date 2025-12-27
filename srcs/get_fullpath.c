/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fullpath.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:01 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/28 01:23:49 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <sys/stat.h>

/* ヘルパー: エラーメッセージを標準エラー出力に出す */
static void	print_error_msg(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
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

/* 絶対パス・相対パス (例: /bin/ls, ./a.out) のチェック */
static char	*check_absolute_path(char *path, t_alloc *heap)
{
	struct stat	path_stat;
	char		*result;

	// 1. 存在チェック
	if (access(path, F_OK) != 0)
	{
		print_error_msg(path, "No such file or directory");
		return (NULL);
	}
	// 2. ディレクトリかどうか
	// (accessが成功したので stat も成功するはずだが、一応 errno は使わない)
	stat(path, &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		print_error_msg(path, "Is a directory");
		return (NULL);
	}
	// 3. 実行権限チェック
	if (access(path, X_OK) != 0)
	{
		print_error_msg(path, "Permission denied");
		return (NULL);
	}
	// 4. すべてクリアなら複製して返す
	result = ft_strdup(path);
	if (!result)
		(cleanup(heap), exit(1));
	return (result);
}

/* PATH環境変数からの探索 */
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

		if (access(fullpath, F_OK) == 0) // ファイルが見つかった場合
		{
			if (access(fullpath, X_OK) == 0) // 実行可能ならそれを返す
				return (fullpath);

			// 見つかったが実行権限がない -> 検索を中止してエラーを出す (Bashの挙動)
			print_error_msg(cmd_name, "Permission denied");
			free(fullpath);
			return (NULL);
		}
		free(fullpath);
		i++;
	}
	// ループを抜けた = どこにもなかった
	print_error_msg(cmd_name, "command not found");
	return (NULL);
}

char	*get_fullpath(char *cmd_name, t_alloc *heap)
{
	char	**bin_dir;
	char	*fullpath;
	char	*envp_path;

	if (!cmd_name)
		return (NULL);

	// 1. パス区切り文字が含まれる場合 (/ や ./) -> 直接チェック
	if (ft_strchr(cmd_name, '/'))
		return (check_absolute_path(cmd_name, heap));

	// 2. PATH環境変数が無い、または空の場合
	envp_path = search_get_env(heap->ev_clone, "PATH");
	if (!envp_path || ft_strlen(envp_path) == 0)
	{
		print_error_msg(cmd_name, "No such file or directory");
		return (NULL);
	}

	// 3. PATHを分割して探索
	bin_dir = ft_split(envp_path, ':');
	if (!bin_dir)
		(cleanup(heap), exit(1));

	fullpath = check_path_and_perm(bin_dir, cmd_name, heap);

	free_2d_array(&bin_dir);
	return (fullpath);
}
