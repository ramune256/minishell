/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/28 20:05:14 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	import_signal_status(t_alloc *heap)
{
	if (g_sig_status)
	{
		heap->exit_status = 128 + SIGINT;
		g_sig_status = 0;
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
	t_alloc	heap;

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
