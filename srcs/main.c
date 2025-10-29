/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/29 16:11:01 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **ep)
{
	t_alloc		heap;
	// t_token		*tmp;

	(void)ac;
	(void)av;
	ft_bzero(&heap, sizeof(t_alloc));
	while (1)
	{
		heap.line = readline("minishell> ");
		if (heap.line == NULL)
		{
			printf("exit\n");
			break;
		}
		if (*heap.line)
			add_history(heap.line);

		// 1. Tokenize
		tokenize(&heap, heap.line);

		// 2. Parse
		parse(&heap);
		// tmp = heap.head; // ポインタを消費するためコピーを渡す
		// heap.ast = parse(&tmp);

		// 3. Execute
		if (heap.ast)
		{
			execute();
			// printf("--- AST ---\n");
			// print_ast(alloc.ast, 0);
			// printf("-----------\n");
		}

		// 4. Cleanup
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
