#ifndef _SIGNAL_H_
# define _SIGNAL_H_

# define _GNU_SOURCE

# include <limits.h>
# include <signal.h>

void defalt_signal(void);
void signal_readline_reset(void);
void init_signal(void);
static void	handler(int signal);

#endif