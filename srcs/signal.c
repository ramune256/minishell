/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:59:04 by shunwata          #+#    #+#             */
/*   Updated: 2025/12/18 16:30:46 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_sig_status = 0;

/*
** 入力待ち（インタラクティブモード）用のハンドラ
** Ctrl-C -> 改行して新しいプロンプトを表示
*/
void	handle_sigint(int sig)
{
	(void)sig;
	g_sig_status = 1;      // フラグを立てる（終了ステータスの更新などに使う）
	write(STDOUT_FILENO, "\n", 1);     // 改行
	rl_on_new_line();      // readlineに「新しい行にいるよ」と伝える
	rl_replace_line("", 0); // 入力中のバッファをクリア
	rl_redisplay();        // プロンプトを再描画
}

/*
** 1. 初期化・入力待ち用 (init_signal)
*/
void	set_signal_shell(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	// ★注意: readline使用時は SA_RESTART を外すことが多いですが、
	// 上記ハンドラ内で rl_redisplay しているので SA_RESTART があってもなくても
	// 動作しますが、システムコールの挙動をBashに近づけるなら外すのが無難です。
	// 相方さんの SA_RESTART でもこのハンドラなら動きます。

	sigaction(SIGINT, &sa, NULL);

	// Ctrl-\ (SIGQUIT) は何もしない
	signal(SIGQUIT, SIG_IGN);
}

/*
** 2. 子プロセス用 (kid_signal)
** execveの前などで呼ぶ
*/
void	set_signal_child(void)
{
	// Ctrl-C: デフォルト動作（プロセス終了）に戻す
	signal(SIGINT, SIG_DFL);

	// ★修正: Ctrl-\: デフォルト動作（Quit / Core Dump）に戻す
	signal(SIGQUIT, SIG_DFL);
}

/*
** 3. 親プロセス用 (par_signal)
** 子プロセスの実行を待っている間 (blocking)
*/
void	set_signal_parent(void)
{
	// 実行中は親はシグナルで余計な出力をしないように無視する
	// (Ctrl-C等のシグナルは端末からプロセスグループ全体に送られるので、
	//  子プロセスが受け取って終了し、親はwaitpidでそれを検知する流れが正解)
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
