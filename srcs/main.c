/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/27 20:52:15 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	t_alloc		alloc;
	t_token		*tmp;

	ft_bzero(&alloc, sizeof(t_alloc));
	while (1)
	{
		alloc.line = readline("minishell> ");
		if (alloc.line == NULL)
		{
			printf("exit\n");
			break;
		}
		if (*alloc.line)
			add_history(alloc.line);

		// 1. Tokenize
		tokenize(&alloc, alloc.line);

		// 2. Parse
		tmp = alloc.head; // ポインタを消費するためコピーを渡す
		alloc.ast = parse(&tmp);

		// 3. Print AST for debugging
		if (alloc.ast)
		{
			printf("--- AST ---\n");
			print_ast(alloc.ast, 0);
			printf("-----------\n");
		}

		// 4. Cleanup
		cleanup(&alloc);
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
