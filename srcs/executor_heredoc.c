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
		unlink((char *)current->content); // 一時ファイルを削除
		free(current->content); // strdupしたファイル名を解放
		free(current); // リストのノードを解放
		current = tmp;
	}
	*list = NULL;
}

static bool	put_line_to_tmpfile(char *line,char *delimiter, int tmp_fd)
{
	bool	atty;
	bool	is_delimiter;
	size_t	len;

	atty = isatty(STDIN_FILENO);
	len = ft_strlen(delimiter);
	if (atty)
		is_delimiter = (ft_strcmp(line, delimiter) == 0);
	else
		is_delimiter = (ft_strncmp(line, delimiter, len) == 0 && line[len] == '\n');
	if (is_delimiter)
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
	return (filename); // 例: /tmp/.minishell_heredoc_0
}

// ヒアドキュメントの入力を読み取る
static void	read_heredoc_input(t_cmd *node, t_alloc *heap)
{
	char	*line;
	int		tmp_fd;
	char	*delimiter;
	char	*temp_filename;

	delimiter = node->file;
	temp_filename = generate_temp_filename(heap);

	// 一時ファイルを書き込みモードで開く
	tmp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd == -1)
		(perror("open"), cleanup(heap), exit(1));

	// 区切り文字が入力されるまで readline で読み込む
	while (1)
	{
		get_input(&line, "> ");
		if (line == NULL) // EOF (Ctrl+D)
			break;
		if (put_line_to_tmpfile(line, delimiter, tmp_fd))
		{
			free(line); // 最後に line を free して break
			break;
		}
		free(line);
	}
	close(tmp_fd);

	// ノードを書き換える
	free(node->file); // 元の "EOF" (区切り文字) を解放
	node->file = temp_filename; // 新しい一時ファイル名を設定
	node->mode = O_RDONLY;      // modeをTOKEN_HEREDOCからO_RDONLYに変更
	// node->fd は STDIN_FILENO のまま

	// ★ 後でunlinkするために、一時ファイル名をリストに追加
	ft_lstadd_back(&heap->temp_files, ft_lstnew(ft_strdup(temp_filename)));
}

// ASTを再帰的に探索し、ヒアドキュメントを処理する
void	find_and_process_heredocs(t_cmd *ast, t_alloc *heap)
{
	if (!ast)
		return ;

	// パイプの場合は、左右両方を探索
	if (ast->type == NODE_PIPE)
	{
		find_and_process_heredocs(ast->left, heap);
		find_and_process_heredocs(ast->right, heap);
	}
	// リダイレクトの場合
	else if (ast->type == NODE_REDIR)
	{
		if (ast->mode == TOKEN_HEREDOC)
			read_heredoc_input(ast, heap); // ★ ヒアドキュメントを見つけたら、入力を読み取る
		// サブコマンドも再帰的に探索
		find_and_process_heredocs(ast->subcmd, heap);
	}
	// EXECの場合は何もしない
}
