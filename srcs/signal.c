/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:59:04 by shunwata          #+#    #+#             */
/*   Updated: 2026/01/02 21:41:17 by nmasuda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_status = 0;

void	handle_sigint(int sig)
{
	int	dummy;

	(void)sig;
	g_sig_status = 1;
	dummy = write(STDOUT_FILENO, "\n", 1);
	(void)dummy;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_heredoc(int sig)
{
	int	dummy;

	(void)sig;
	g_sig_status = 1;
	dummy = write(STDOUT_FILENO, "\n", 1);
	(void)dummy;
	close(STDIN_FILENO);
}

void	set_signal_shell(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signal_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	set_signal_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
