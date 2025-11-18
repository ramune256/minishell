
#include "../includes/signal.h"

static volatile sig_atomic_t g_signal_flag = 0;

static void	handler(int signal)//引数返り値固定 int main(void) みたいな感じ
{
    (void)signal;
	g_signal_flag = 1;
}

void init_signal(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);//デフォルトである構造体の中身を空に
	sa.sa_handler = handler;//signalが来た場合どこの関数にsignalが来たよーってするかの設定
	sa.sa_flags = SA_RESTART;//flag readlineのシステムコールがcntrl-Cで止まらないようにする
    signal(SIGQUIT,SIG_IGN);//問題文からcntrl-\は何もしないためSIG_IGNで無効設定に
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		printf("error\n");
	}
}

void signal_readline_reset(void)//もしreadlineの途中にシグナルが来た場合、readlineの内容をリセットする
{
	if (isatty(STDIN_FILENO))
	{
        write(STDOUT_FILENO,"\n",1);
		rl_on_new_line();//新しい行に移動したことを伝える
		rl_replace_line("",0);//バッファを空に
		rl_redisplay();//minishell>の再描画
	}
	g_signal_flag = 0;//signalが来たよーの合図をリセット
}


//修正
void defalt_signal(void)
{
	signal(SIGINT,SIG_DFL);
    signal(SIGQUIT,SIG_IGN);//??
    //標準動作
}