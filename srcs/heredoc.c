/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:51:08 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:00:29 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

static void	set_heredoc_file(t_cmd *node, char *tmp_filename, t_mshell *data)
{
	char	*copy;
	t_list	*list_node;

	free(node->file);
	node->file = tmp_filename;
	node->mode = O_RDONLY;
	copy = ft_strdup(tmp_filename);
	if (!copy)
		(cleanup(data), exit(1));
	list_node = ft_lstnew(copy);
	if (!list_node)
		(free(copy), cleanup(data), exit(1));
	ft_lstadd_back(&data->tmp_files, list_node);
}

static bool	read_heredoc_input(char **line, const char *message)
{
	void	(*tmp_handle_sigint)(int);

	*line = NULL;
	tmp_handle_sigint = signal(SIGINT, handle_heredoc);
	if (isatty(STDIN_FILENO))
		*line = readline(message);
	else
		*line = get_next_line(STDIN_FILENO);
	signal(SIGINT, tmp_handle_sigint);
	if (g_sig_status)
	{
		if (*line)
			free(*line);
		*line = NULL;
		return (false);
	}
	return (true);
}

static void	write_heredoc_lines(const char *del, int tmp_filefd)
{
	char	*line;

	line = NULL;
	while (1)
	{
		if (read_heredoc_input(&line, "> ") == false)
			return ;
		if (line == NULL || is_delimiter(line, del))
		{
			free(line);
			return ;
		}
		ft_putstr_fd(line, tmp_filefd);
		if (isatty(STDIN_FILENO))
			ft_putstr_fd("\n", tmp_filefd);
		free(line);
	}
}

static void	make_heredoc_file(t_cmd *node, t_mshell *data)
{
	int		stdin_backup;
	int		tmp_filefd;
	char	*tmp_filename;

	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		(perror("dup"), cleanup(data), exit(1));
	tmp_filename = generate_tmp_filename(data);
	tmp_filefd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (tmp_filefd == -1)
		(perror("open"), cleanup(data), exit(1));
	write_heredoc_lines(node->file, tmp_filefd);
	close(tmp_filefd);
	if (g_sig_status)
	{
		(dup2(stdin_backup, STDIN_FILENO), close(stdin_backup));
		(unlink(tmp_filename), free(tmp_filename));
		return ;
	}
	(dup2(stdin_backup, STDIN_FILENO), close(stdin_backup));
	set_heredoc_file(node, tmp_filename, data);
}

void	heredoc(t_cmd *node, t_mshell *data)
{
	if (!node || g_sig_status)
		return ;
	if (node->type == NODE_PIPE)
	{
		heredoc(node->left, data);
		if (g_sig_status)
			return ;
		heredoc(node->right, data);
	}
	else if (node->type == NODE_REDIR)
	{
		heredoc(node->subcmd, data);
		if (g_sig_status)
			return ;
		if (node->mode == TOKEN_HEREDOC)
		{
			make_heredoc_file(node, data);
			if (g_sig_status)
				return ;
		}
	}
}
