/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:43:26 by nmasuda           #+#    #+#             */
/*   Updated: 2026/02/15 00:11:51 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	echo_print(char **line, char **av, int ac);

static bool	special_vars(char *tmp, char **av, int ac, char **cur)
{
	int	i;

	i = 0;
	if (**cur == '$')
		i = getpid();
	else if (**cur == '#')
		i = ac - 1;
	if (**cur == '@')
		echo_print(av + 1, NULL, 0);
	else if ((**cur == '0' || ft_isdigit(**cur)) && **cur - '0' < ac)
		ft_putstr_fd(av[**cur - '0'], STDOUT_FILENO);
	else if (**cur == '#' || **cur == '$')
	{
		tmp = ft_itoa(i);
		if (!tmp)
			return (false);
		(ft_putstr_fd(tmp, STDOUT_FILENO), free(tmp));
	}
	else
		write(STDOUT_FILENO, "$", 1);
	return ((*cur)++, true);
}

static bool	check_doller_and_put_line(char *line, char **av, int ac)
{
	char	*cur;
	char	*start;

	start = line;
	while (ac)
	{
		cur = ft_strchr(start, '$');
		if (!cur)
			break ;
		write(STDOUT_FILENO, start, cur - start);
		cur++;
		if (special_vars(NULL, av, ac, &cur) == false)
			return (false);
		start = cur;
	}
	ft_putstr_fd(start, STDOUT_FILENO);
	return (true);
}

static bool	echo_print(char **line, char **av, int ac)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (check_doller_and_put_line(line[i], av, ac) == false)
			return (false);
		if (line[i])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	return (true);
}

static bool	is_n_opt(const char *arg)
{
	int	i;

	if (!arg || ft_strncmp(arg, "-n", 2))
		return (false);
	i = ft_strlen("-n");
	while (arg[i] == 'n')
		i++;
	if (arg[i] != '\0')
		return (false);
	return (true);
}

int	c_echo(char **line, t_alloc *heap)
{
	int	i;

	i = 1;
	(void)heap;
	if (!line)
		return (1);
	if (!line[1])
		ft_putstr_fd("\n", STDOUT_FILENO);
	else if (is_n_opt(line[1]))
	{
		while (line[i] && is_n_opt(line[i]))
			i++;
		if (echo_print(line + i, heap->av, heap->ac) == false)
			return (1);
	}
	else
	{
		if (echo_print(line + 1, heap->av, heap->ac) == false)
			return (1);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (0);
}
