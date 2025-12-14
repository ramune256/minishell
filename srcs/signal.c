#include "minishell_signal.h"

volatile sig_atomic_t g_signal_flag = 0;

static void	handler(int signal)
{
	g_signal_flag = signal;
	if (signal == SIGINT)
	{
        write(STDOUT_FILENO,"\n",1);
		rl_on_new_line();
		rl_replace_line("",0);
		rl_redisplay();
	}
}

void init_signal(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
    signal(SIGQUIT,SIG_IGN);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		printf("error\n");
	}
}

void kid_signal(void)
{
	signal(SIGINT,SIG_DFL);
    signal(SIGQUIT,SIG_DFL);
}

void par_signal(void)
{
	signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
}