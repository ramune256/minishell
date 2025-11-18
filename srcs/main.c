/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2025/11/19 07:03:34 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **ev)
{
	t_alloc		heap;

	((void)ac, (void)av);
	ft_bzero(&heap, sizeof(t_alloc));
	clone_ev(ev, &heap);
	while (1)
	{
		get_input(&heap.line, "minishell> ");
		if(g_signal_flag)
			signal_readline_reset();
		if (heap.line)
		{
			if (isatty(STDIN_FILENO))
				add_history(heap.line);
			tokenize(&heap, heap.line);
			parse(&heap);
			if (heap.ast)
				execute(heap.ast, &heap);
		}
		else
			print_exit();
		cleanup(&heap);
	}
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char *line;

// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (line == NULL)
// 			break;
// 		if (*line)
// 			add_history(line);
// 		exec_command(line, envp);
// 		free(line);
// 	}
// 	return (0);
// }
