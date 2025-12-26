# コードの書き方

・関数の名前は、
「動詞」
「動詞+目的語」
のいずれかであるべきである。
良い例: `clone_ev()、cleanup()、execute()、execute_pipe()、set_signal_parent()、get_next_line()、isalnum()`
悪い例: `prefix_add_after()、execute_core()`

・checkやprocess、changeのような具体性の無い単語を関数の名前に使うべきではない。

・関数を分割する目的は「抽象化」であるべきである。
・`int *`等を連れ回すのは「抽象化されていないコードの分割」で、本質的な意味が無い。
・関数は出来るだけ再利用できるように作るべきである。


悪いコードの例
```c
bool	main_create_cur(size_t *ev_len, size_t fir, char *ev_var, char **cur)
{
	size_t	before;
	size_t	after;
	char	*prefix;
	char	*tmp;

	before = *ev_len;
	after = ft_strlen(ev_var);
	prefix = NULL;
	tmp = NULL;
	prefix = ft_substr(*cur, 0, fir);
	if (!prefix)
		return (free(*cur), free(ev_var), false);
	tmp = ft_strjoin(prefix, ev_var);
	(free(prefix), free(ev_var));
	if (!tmp)
		return (free(*cur), false);
	if (prefix_add_after(fir + before, fir + after, tmp, cur) == false)
		return (false);
	return (true);
}

char	*create_cur(size_t *j, char *cur, t_alloc *heap)
{
	size_t	fir;
	size_t	ev_len;
	char	*ev_var;

	fir = *j;
	ev_len = 0;
	ev_var = NULL;
	(*j)++;
	ev_var = chenge_ev(cur, heap, j, &ev_len);
	if (ev_var)
	{
		if (main_create_cur(&ev_len, fir, ev_var, &cur) == false)
			return (NULL);
	}
	else
		*j = fir + 1;
	return (cur);
}
```

良いコードの例
```c
	while (1)
	{
		get_input(&heap.line, "minishell> ");
		if (heap.line)
		{
			if (isatty(STDIN_FILENO))
				add_history(heap.line);
			tokenize(&heap);
			parse(&heap);
			expand(heap.ast, &heap);
			print_ast(heap.ast, 0);
			execute(heap.ast, &heap);
		}
	}

```
ロジックが抽象化されている

・その関数で変更しないポインタ引数にはなるべく`static`を付け、変更しないものには付けない
関数の意図が分かりやすくなる
ただし読みやすさを損なうのなら付ける必要はない

・ぱっと見で元の単語が理解できない場合、省略しない
例: ret、par、cur、cnt

・`malloc`などの関数の失敗は重大なエラーであり、上位関数に伝播するよりはその場で即時終了させることが望ましい。
`cleanup()`のような関数と、動的に確保したメモリへのポインタをまとめた構造体を作成し、コードのどこにいても即座に全てのメモリを解放し`exit()`できるような仕組みが必要である。
ただし、`ft_strdup()`のような再利用性の高い関数の場合は即時終了させる必要はない。


# NORMINETTE C言語コーディング規約

## フォーマット
1. インデントは全て「水平タブ」を使用する（スペース禁止）。
2. 各行は80文字以内。長すぎる行はバックスラッシュ `\` で改行し、適切にインデントする。
3. 関数間は「1つの空行」で区切る。
4. 空行に空白文字やタブを含めない。
5. 関数定義時、戻り値の型と関数名の間は「水平タブ」1つとする。

## 変数
6. 変数宣言は必ず関数の冒頭に行う。
7. 1つの関数内で宣言できる変数は「最大5個」まで。
8. 変数名はスコープ内で同じ列に揃えてインデントする。
9. 変数宣言は1行につき1つ。
10. 変数宣言と同時に初期化をしてはならない（例: `int a = 0;` は禁止）。

## 制御構文・禁止事項
11. `for`文は使用禁止（`while`を使用すること）。
12. 三項演算子（`? :`）は使用禁止。
13. `while` や `if` の条件式内で代入を行わない。
14. `while` と `{` を同じ行に書かない。

## return
15. `return` の値は `()` で囲む。`return` と `()` の間にスペースを1つ入れる（例: `return (0);`）。
16. `void`関数の場合は `return ;` と書く。

## その他
17. `if`文の中括弧で無駄に行数を使うのを防ぐために、`if`の中身は一行で書ける事が望ましい。
18. 複数関数を一行で実行したい場合、`(cleanup(&something), exit(1));`のような書き方は許容される。
19. `return ()`の括弧の中に関数を入れてコードの行数を省略できる場合、そのようにするべきである。（コンマ演算子を使う）
20. 全ての関数は抽象化によって高い可読性を有しているべきであり、関数の中身の長さはそれぞれ25行以内であるべきである。
