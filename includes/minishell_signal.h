#ifndef _MINISHELL_SIGNAL_H_
# define _MINISHELL_SIGNAL_H_

# define _GNU_SOURCE

# include <limits.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>

extern volatile sig_atomic_t g_signal_flag;

void par_signal(void);
void kid_signal(void);
void init_signal(void);

#endif