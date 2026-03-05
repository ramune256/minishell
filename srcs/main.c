/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:00:21 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

static void	interpret(t_mshell *data)
{
	if (isatty(STDIN_FILENO))
		add_history(data->line);
	tokenize(data);
	parse(data);
	expand(data->node, data);
	heredoc(data->node, data);
	execute(data->node, data);
}

int	main(int ac, char **av, char **ev)
{
	t_mshell	data;

	ft_bzero(&data, sizeof(t_mshell));
	clone_ev(ev, &data);
	data.ac = ac;
	data.av = av;
	set_signal_shell();
	while (1)
	{
		get_input(&data.line, "minishell> ");
		import_signal_status(&data);
		if (data.line)
			interpret(&data);
		else
			print_exit(&data);
		data.success = true;
		cleanup(&data);
	}
	return (0);
}
