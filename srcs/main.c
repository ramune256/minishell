/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/23 20:28:54 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ast(t_cmd *cmd, int level)
{
	if (cmd == NULL)
		return ;
	for (int i = 0; i < level; i++)
		printf("  ");
	if (cmd->type == NODE_EXEC)
	{
		printf("EXEC: ");
		for (int i = 0; cmd->argv[i]; i++)
			printf("[%s] ", cmd->argv[i]);
		printf("\n");
	}
	else if (cmd->type == NODE_PIPE)
	{
		printf("PIPE\n");
		print_ast(cmd->left, level + 1);
		print_ast(cmd->right, level + 1);
	}
	else if (cmd->type == NODE_REDIR)
	{
		printf("REDIR (mode: %d, fd: %d, file: %s)\n", cmd->mode, cmd->fd, cmd->file);
		print_ast(cmd->subcmd, level + 1);
	}
}

static void	interpret(t_alloc *heap)
{
	if (isatty(STDIN_FILENO))
		add_history(heap->line);
	tokenize(heap);
	parse(heap);
	expand(heap->ast, heap);
	print_ast(heap->ast, 0);
	execute(heap->ast, heap);
}

int	main(int ac, char **av, char **ev)
{
	t_alloc		heap;

	((void)ac, (void)av);
	ft_bzero(&heap, sizeof(t_alloc));
	clone_ev(ev, &heap);
	set_signal_shell();
	while (1)
	{
		get_input(&heap.line, "minishell> ");
		import_signal_status(&heap);
		if (heap.line)
			interpret(&heap);
		else
			print_exit(&heap);
		heap.success = true;
		cleanup(&heap);
	}
	return (0);
}
