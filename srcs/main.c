/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/28 01:49:19 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (g_sig_status)
		{
			heap.exit_status = 130;
			g_sig_status = 0;
		}
		if (heap.line)
		{
			if (isatty(STDIN_FILENO))
				add_history(heap.line);
			tokenize(&heap);
			parse(&heap);
			expand(heap.ast, &heap);
			execute(heap.ast, &heap);
		}
		else
			print_exit(&heap);
		heap.success = true;
		cleanup(&heap);
	}
	return (0);
}
