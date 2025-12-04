#include "minishell_signal.h"

volatile sig_atomic_t g_signal_flag = 0;

static void	handler(int signal)//引数返り値固定 int main(void) みたいな感じ
{
	g_signal_flag = signal;
	if (signal == SIGINT)
	{
        write(STDOUT_FILENO,"\n",1);
		rl_on_new_line();//新しい行に移動したことを伝える
		rl_replace_line("",0);//バッファを空に
		rl_redisplay();//minishell>の再描画
	}
}

void init_signal(void)
{
	struct sigaction	sa;
	struct termios      term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

	sigemptyset(&sa.sa_mask);//デフォルトである構造体の中身を空に
	sa.sa_handler = handler;//signalが来た場合どこの関数にsignalが来たよーってするかの設定
	sa.sa_flags = SA_RESTART;//flag readlineのシステムコールがcntrl-Cで止まらないようにする
    signal(SIGQUIT,SIG_IGN);//問題文からcntrl-\は何もしないためSIG_IGNで無効設定に
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		printf("error\n");
	}
}



//修正
void kid_signal(void)
{
	signal(SIGINT,SIG_DFL);//cntrl-Cを標準動作に
    signal(SIGQUIT,SIG_IGN);
}

void par_signal(void)
{
	signal(SIGINT,SIG_IGN);//cntrl-Cを無視設定に
    signal(SIGQUIT,SIG_IGN);
}