/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:51:08 by shunwata          #+#    #+#             */
/*   Updated: 2026/01/28 16:24:40 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_temp_files(t_list **list)
{
	t_list	*current;
	t_list	*tmp;

	current = *list;
	while (current)
	{
		tmp = current->next;
		unlink((char *)current->content);
		free(current->content);
		free(current);
		current = tmp;
	}
	*list = NULL;
}

static bool	put_line_to_tmpfile(char *line, char *delimiter, int tmp_fd)
{
	bool	atty;
	bool	is_del;
	size_t	len;

	atty = isatty(STDIN_FILENO);
	len = ft_strlen(delimiter);
	if (atty)
		is_del = (ft_strcmp(line, delimiter) == 0);
	else
		is_del = (ft_strncmp(line, delimiter, len) == 0 && line[len] == '\n');
	if (is_del)
		return (true);
	ft_putstr_fd(line, tmp_fd);
	if (atty)
		ft_putstr_fd("\n", tmp_fd);
	return (false);
}

static char	*generate_temp_filename(t_alloc *heap)
{
	static int	id = 0;
	char		*num;
	char		*filename;

	num = ft_itoa(id++);
	if (!num)
		(cleanup(heap), exit(1));
	filename = ft_strjoin("/tmp/.minishell_heredoc_", num);
	free(num);
	if (!filename)
		(cleanup(heap), exit(1));
	return (filename);
}

static bool	get_heredoc_input(char **line, const char *message)
{
    void    (*tmp_handle_sigint)(int);

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

static void    read_heredoc_input(t_cmd *node, t_alloc *heap)
{
    char	*line;
    int		tmp_fd;
    char	*fn;
    int		stdin_backup;

    line = NULL;
    stdin_backup = dup(STDIN_FILENO);
    fn = generate_temp_filename(heap);
    tmp_fd = open(fn, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (tmp_fd == -1)
        (perror("open"), cleanup(heap), exit(1));
    while (1)
    {
        if (get_heredoc_input(&line, "> ") == false)
        {
            dup2(stdin_backup, STDIN_FILENO);
            (close(stdin_backup), close(tmp_fd));
			free(fn);
            return ;
        }
        if (line == NULL)
            break ;
        if (put_line_to_tmpfile(line, node->file, tmp_fd))
        {
            free(line);
            break ;
        }
        free(line);
    }
    (close(tmp_fd), close(stdin_backup));
    free(node->file);
    node->file = fn;
    node->mode = O_RDONLY;
    ft_lstadd_back(&heap->temp_files, ft_lstnew(ft_strdup(fn)));
}

void	find_and_process_heredocs(t_cmd *node, t_alloc *heap)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		find_and_process_heredocs(node->left, heap);
		find_and_process_heredocs(node->right, heap);
	}
	else if (node->type == NODE_REDIR)
	{
		if (node->mode == TOKEN_HEREDOC)
			read_heredoc_input(node, heap);
		find_and_process_heredocs(node->subcmd, heap);
	}
}
