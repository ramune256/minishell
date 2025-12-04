/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/04 19:33:50 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

int	main(int ac, char **av, char **ev)
{
	t_alloc		heap;

	((void)ac, (void)av);
	ft_bzero(&heap, sizeof(t_alloc));
	clone_ev(ev, &heap);
	init_signal();
	while (1)
	{
		get_input(&heap.line, "minishell> ");
		if (heap.line)
		{
			if (isatty(STDIN_FILENO))
				add_history(heap.line);
			tokenize(&heap);
			parse(&heap);
			execute(heap.ast, &heap);
		}
		else
		{
			if(g_signal_flag == 0)
			{
				print_exit();
				break;
			}
			else
			{
				heap.exit_status = 130; 
				g_signal_flag = 0;
			}
		}
		heap.success = true;
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
