/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2026/02/25 20:13:00 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

static void	interpret(t_alloc *heap)
{
	if (isatty(STDIN_FILENO))
		add_history(heap->line);
	tokenize(heap);
	parse(heap);
	expand(heap->node, heap);
	heredoc(heap->node, heap);
	execute(heap->node, heap);
}

int	main(int ac, char **av, char **ev)
{
	t_alloc		heap;

	ft_bzero(&heap, sizeof(t_alloc));
	clone_ev(ev, &heap);
	heap.ac = ac;
	heap.av = av;
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
