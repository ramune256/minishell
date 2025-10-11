/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2025/10/11 21:44:08 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*ast;

	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
		{
			printf("exit\n");
			break;
		}
		if (*line)
			add_history(line);

		// 1. Tokenize
		tokens = tokenize(line);

		// 2. Parse
		t_token *tmp = tokens; // ポインタを消費するためコピーを渡す
		ast = parse(&tmp);

		// 3. Print AST for debugging
		if (ast)
		{
			printf("--- AST ---\n");
			print_ast(ast, 0);
			printf("-----------\n");
		}

		// 4. Cleanup
		free_ast(ast);
		free_tokens(tokens);
		free(line);
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
