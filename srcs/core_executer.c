/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_executer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:31 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/09 21:23:44 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// // NODE_EXEC を実行する
// static void	execute_exec(t_alloc *heap, t_cmd *ast, char **envp)
// {
// 	pid_t	pid;
// 	char	*fullpath;

// 	fullpath = get_fullpath(ast->argv[0], envp, heap);
// 	pid = fork();
// 	if (pid == -1)
// 		(perror("fork"), cleanup(heap), exit(1));
// 	if (pid == 0)
// 	{
// 		if (execve(fullpath, ast->argv, envp) == -1)
// 			(perror(ast->argv[0]), exit(127)); //not foundは127
// 	}
// 	waitpid(pid, NULL, 0); // TODO: 終了ステータスを受け取る
// }

// static void	execute_exec(t_cmd *ast, t_alloc *heap, char **ev)
// {
// 	pid_t	pid;
// 	char	*fullpath;

// 	pid = fork();
// 	if (pid == -1)
// 		(perror("fork"), cleanup(heap), exit(1));
// 	if (pid == 0)
// 	{
// 		fullpath = get_fullpath(ast->argv[0], ev, heap);
// 		if (fullpath == NULL)
// 			(cleanup(heap), exit(127));
// 		if (execve(fullpath, ast->argv, ev) == -1)
// 			(perror(ast->argv[0]), cleanup(heap), exit(126));
// 	}
// 	waitpid(pid, NULL, 0); // 親プロセスは待つだけ
// }

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
		line = readline("> ");
		if (line == NULL || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		// write(tmp_fd, line, ft_strlen(line));
		// write(tmp_fd, "\n", 1);
		ft_putendl_fd(line, tmp_fd);
		free(line);
	}
	close(tmp_fd);

	// ★ ASTノードを「書き換える」
	free(node->file); // 元の "EOF" (区切り文字) を解放
	node->file = temp_filename; // 新しい一時ファイル名を設定
	node->mode = O_RDONLY;      // modeをTOKEN_HEREDOCからO_RDONLYに変更
	// node->fd は STDIN_FILENO のまま

	// ★ 後でunlinkするために、一時ファイル名をリストに追加
	ft_lstadd_back(&heap->temp_files, ft_lstnew(ft_strdup(temp_filename)));
}

// ASTを再帰的に探索し、ヒアドキュメントを処理する
static void	find_and_process_heredocs(t_cmd *ast, t_alloc *heap)
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

static void	cleanup_temp_files(t_list **list)
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

static bool	is_parent_builtin(t_cmd *ast)
{
	char	*cmd;

	if (ast == NULL || ast->type != NODE_EXEC || ast->argv == NULL)
		return (false);
	cmd = ast->argv[0];
	if (cmd == NULL)
		return (false);
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit", 4) == 0)
		return (true);
	if (ft_strncmp(cmd, "export", 6) == 0 && ast->argv[1] != NULL)
		return (true); // 引数ありのexport
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return (true);
	return (false);
}

static bool	execute_builtin(t_cmd *exec_node, t_alloc *heap, char **envp)
{
	char	*cmd;
	int		status_code;

	if (exec_node == NULL || exec_node->argv == NULL)
		return (0);
	cmd = exec_node->argv[0];
	if (cmd == NULL)
		return (0);
	status_code = 0; // TODO: 終了ステータスを heap->exit_status に保存

	// if (ft_strncmp(cmd, "echo", 4) == 0)
	// 	status_code = c_echo(exec_node->argv);
	// else if (ft_strncmp(cmd, "cd", 2) == 0)
	// 	status_code = c_cd(exec_node->argv, envp); // (実装によります)
	// else if (ft_strncmp(cmd, "pwd", 3) == 0)
	// 	status_code = c_pwd();
	// else if (ft_strncmp(cmd, "export", 6) == 0)
	// 	status_code = c_export(exec_node->argv, envp); // (実装によります)
	// else if (ft_strncmp(cmd, "unset", 5) == 0)
	// 	status_code = c_unset(exec_node->argv, envp); // (実装によります)
	// else if (ft_strncmp(cmd, "env", 3) == 0)
	// 	status_code = c_env(envp);
	// else if (ft_strncmp(cmd, "exit", 4) == 0)
	// 	c_exit(exec_node->argv, heap); // exitは戻らない
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		status_code = c_pwd(exec_node->argv);
	else
		return (false); // ビルトインではなかった

	// (TODO: status_code を heap->exit_status に保存する処理)
	return (true); // ビルトインを実行した
}

static t_cmd	*handle_redirections(t_cmd *ast, t_alloc *heap)
{
	int	file_fd;

	if (ast->type == NODE_EXEC) // ベースケース：EXECノードに到達したら、それを返す
		return (ast);
	if (ast->subcmd->type == NODE_REDIR) // ASTが REDIR(REDIR(EXEC)) の場合、再帰的に処理
		handle_redirections(ast->subcmd, heap);
	file_fd = open(ast->file, ast->mode, 0644); // 0644 = rw-r--r--
	if (file_fd == -1)
		(perror(ast->file), cleanup(heap), exit(1)); // 子プロセスを終了
	if (dup2(file_fd, ast->fd) == -1)
		(perror("dup2"), cleanup(heap), exit(1));
	close(file_fd); // dup2したので、元のFDは不要
	return (ast->subcmd);
}

static void	execute_simple_command(t_cmd *ast, t_alloc *heap, char **ev)
{
	pid_t	pid;
	t_cmd	*exec_node;
	char	*fullpath;

	if (ast->type != NODE_PIPE)
		find_and_process_heredocs(ast, heap);
	pid = fork();
	if (pid == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid == 0)
	{
		exec_node = handle_redirections(ast, heap);
		if (execute_builtin(exec_node, heap, ev))
		{
			// ビルトインが実行されたら、子プロセスはここで終了
			(cleanup(heap), exit(0)); // TODO: 本当は終了ステータスを渡す
		}
		fullpath = get_fullpath(exec_node->argv[0], ev, heap);
		if (fullpath == NULL)
			(cleanup(heap), exit(127));
		if (execve(fullpath, exec_node->argv, ev) == -1)
			(perror(fullpath), free(fullpath), cleanup(heap), exit(126));
	}
	waitpid(pid, NULL, 0);
	cleanup_temp_files(&heap->temp_files);
}

static void	change_fd(int pipefd[2], int target_fd, int fd_num)
{
	dup2(fd_num, target_fd);
	close(pipefd[0]);
	close(pipefd[1]);
}

static void	execute_pipe(t_cmd *ast, t_alloc *heap, char **envp)
{
	int		pipefd[2]; // [0]は読み込み口, [1]は書き込み口
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(pipefd) == -1)
		(perror("pipe"), cleanup(heap), exit(1)); // パイプ作成失敗は致命的
	pid_left = fork(); // 1. 左側の子プロセス (パイプに書き込む側)
	if (pid_left == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid_left == 0)
	{
		change_fd(pipefd, STDOUT_FILENO, pipefd[1]);
		execute(ast->left, heap, envp); // 左側のASTを再帰的に実行
		(cleanup(heap), exit(0)); // 子プロセスをクリーンアップ
	}
	pid_right = fork(); // 2. 右側の子プロセス (パイプから読み込む側)
	if (pid_right == -1)
		(perror("fork"), cleanup(heap), exit(1));
	if (pid_right == 0)
	{
		change_fd(pipefd, STDIN_FILENO, pipefd[0]);
		execute(ast->right, heap, envp); // 右側のASTを再帰的に実行
		(cleanup(heap), exit(0)); // 子プロセスをクリーンアップ
	}
	(close(pipefd[0]), close(pipefd[1]));
	(waitpid(pid_left, NULL, 0), waitpid(pid_right, NULL, 0)); // TODO: 終了ステータス
}

// ASTを再帰的にたどって実行するエントリーポイント
void	execute(t_cmd *ast, t_alloc *heap, char **ev)
{
	if (ast == NULL)
		return ;
	if (ast->type == NODE_PIPE)
		execute_pipe(ast, heap, ev);
	else if (ast->type == NODE_REDIR)
		execute_simple_command(ast, heap, ev);
	else if (ast->type == NODE_EXEC)
	{
		if (is_parent_builtin(ast))
			execute_builtin(ast, heap, ev); // forkせずに、親プロセスでそのまま実行
		else
			execute_simple_command(ast, heap, ev); // forkする
	}
}

// int	exec_command(char *input, char **envp)
// {
// 	pid_t			pid;
// 	char			**cmd_args;
// 	char			*cmd_fullpath;
// 	t_split_err		split_err;
// 	t_get_fpath_err	get_fpath_err;

// 	cmd_args = split_improved(input, &split_err);
// 	if (split_err == SYNTAX_ERR)
// 		something();
// 	if (!cmd_args)
// 		malloc_failed();
// 	cmd_fullpath = get_fullpath(cmd_args, envp, &get_fpath_err);
// 	if (!cmd_fullpath)
// 	{

// 	}
// 	fork();
// 	execve(cmd_fullpath, cmd_args, envp);
// 	return (free(cmd_fullpath), free_2d_array(cmd_args), 0);
// }
