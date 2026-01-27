/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:51:08 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/14 18:45:14 by shunwata         ###   ########.fr       */
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

static void	read_heredoc_input(t_cmd *node, t_alloc *heap)
{
	char	*line;
	int		tmp_fd;
	char	*fn;

	fn = generate_temp_filename(heap);
	tmp_fd = open(fn, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd == -1)
		(perror("open"), cleanup(heap), exit(1));
	while (1)
	{
		get_input(&line, "> ");
		if (line == NULL)
			break ;
		if (put_line_to_tmpfile(line, node->file, tmp_fd))
		{
			free(line);
			break ;
		}
		free(line);
	}
	close(tmp_fd);
	free(node->file);
	node->file = fn;
	node->mode = O_RDONLY;
	ft_lstadd_back(&heap->temp_files, ft_lstnew(ft_strdup(fn)));
}

void	find_and_process_heredocs(t_cmd *ast, t_alloc *heap)
{
	if (!ast)
		return ;
	if (ast->type == NODE_PIPE)
	{
		find_and_process_heredocs(ast->left, heap);
		find_and_process_heredocs(ast->right, heap);
	}
	else if (ast->type == NODE_REDIR)
	{
		if (ast->mode == TOKEN_HEREDOC)
			read_heredoc_input(ast, heap);
		find_and_process_heredocs(ast->subcmd, heap);
	}
}
