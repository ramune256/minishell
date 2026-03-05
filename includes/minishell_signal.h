/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmasuda <nmasuda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 20:01:10 by shunwata          #+#    #+#             */
/*   Updated: 2026/03/04 22:01:53 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SIGNAL_H
# define MINISHELL_SIGNAL_H

# define _GNU_SOURCE

# include <signal.h>

extern volatile sig_atomic_t	g_sig_status;

void	import_signal_status(t_mshell *data);
void	set_signal_shell(void);
void	set_signal_child(void);
void	set_signal_parent(void);
void	handle_heredoc(int sig);
void	handle_sigint(int sig);

#endif
