/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 21:10:07 by shunwata          #+#    #+#             */
/*   Updated: 2026/04/07 17:00:43 by shunwata         ###   ########.fr       */
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

static void	put_ascii(void)
{
	int		fd;
	char	*line;

	if (!isatty(STDIN_FILENO))
		return ;
	fd = open("ascii.txt", O_RDONLY);
	if (fd < 0)
		return ;
	line = get_next_line(fd);
	while (line)
	{
		ft_putstr(line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

int	main(int ac, char **av, char **ev)
{
	t_mshell	data;

	ft_bzero(&data, sizeof(t_mshell));
	clone_ev(ev, &data);
	data.ac = ac;
	data.av = av;
	set_signal_shell();
	put_ascii();
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
