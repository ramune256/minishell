
#include "minishell.h"

char	*tenkai(char *line, char **ev, char *exit_status, int j, int *ev_len)
{
	int		cnt;
	int		ev_num;
	char	*tail;
	if (!line)
		return (NULL);
	if (line[j] == '?')
		return (strdup(exit_status));
	tail = ft_substr(line, (unsigned int)j, strlen(line) - (size_t)j);
	if (!tail)
		return (NULL);
	ev_num = 0;
	while (ev[ev_num])
	{
		cnt = 0;
		while (ev[ev_num][cnt] && ev[ev_num][cnt] != '=')
			cnt++;
		if (ev[ev_num][cnt] == '=')
		{
		    *ev_len = cnt;
		    if (!strncmp(tail, ev[ev_num], cnt))
		    {
			    char *val = ft_substr(ev[ev_num], (unsigned int)(cnt + 1),strlen(ev[ev_num]) - (size_t)(cnt + 1));
			    free(tail);
			    return (val);
		    }
        }
		ev_num++;
	}
	return (free(tail),NULL);
}

int test_check(char **av,char **ev)//引数合わせる
{
    int i = 1;
	int j = 0;
	char *tenkai_no_nakami = NULL;
	int before = 0;
    int saisyo;
    while(av[i])
    {
        while(av[i][j] != '$' && av[i][j])
            j++;
		saisyo = j;
		if(av[i][j] == '$' && av[i][j])
		{
        	j++;//$の分
			tenkai_no_nakami = tenkai(av[i],ev,"exit_status",j,&before);
			j += strlen(tenkai_no_nakami);
		}
		else
		{
			j = 0;
			if(!av[++i])
                break;
		}
		char *res = NULL;
		res = ft_substr(av[i],0,saisyo);//saisyo
		res = ft_strjoin(res,tenkai_no_nakami);//nakami
		res = ft_strjoin(res,ft_substr(av[i],saisyo + before +1,strlen(av[i]) - before+1));//saigo
		av[i] = strdup(res);
        //上三つの中でmalloc使ってるから失敗時freeするように関数分ける
	}
    return 0;
}

void check(t_cmd *ast,t_alloc *heap)
{
	test_check(ast->argv,heap);
}

void expander(t_cmd *ast, t_alloc *heap)
{
    if(ast == NULL)
        return;

    if(ast->type == NODE_EXEC)
        check(ast,heap);

    if(ast->left)
        expander(ast->left,heap);
    if(ast->right)
       expander(ast->right,heap);
}


/*
int	main(int ac, char **av, char **ev)
{
	(void)ac;
    int i = 1;
	while(av[i])
	{
		char *cur = strdup(av[i]);
		if (!cur)
			return (1);
		printf("saisyomoziretu:%s\n", cur);
		int j = 0;
		while (1)
		{
			while (cur[j] && cur[j] != '$')
				j++;
			if (!cur[j])
			{
				printf("owari\n");
				break;
			}
			int saisyo = j;
			j++;
			int ev_len = 0;
			char *tenkai_no_nakami = tenkai(cur, ev, "exit_status", j, &ev_len);
			if(tenkai_no_nakami)
			{
				int before = ev_len;//変数の展開前の長さ
				int after = (int)strlen(tenkai_no_nakami);//変数の展開後の長さ	

				//prefix →　変数前の文字列
				char *prefix = ft_substr(cur, 0, (size_t)saisyo);
				if (!prefix)
				{ free(tenkai_no_nakami); free(cur); return (1); }

				//prefix＋展開後の変数
				char *tmp = ft_strjoin(prefix, tenkai_no_nakami);
				free(prefix);
				if (!tmp) { free(tenkai_no_nakami); free(cur); return (1); }

				int tail_start = saisyo + 1 + before;
				int cur_len = strlen(cur);
				int tail_len = 0;
				if (tail_start <= cur_len)
					tail_len = cur_len - tail_start;
				else
					tail_len = 0;

				char *tail = ft_substr(cur, (unsigned int)tail_start, tail_len);
				if (!tail)
				{ free(tmp); free(tenkai_no_nakami); free(cur); return (1); }

				//res = tmp(prefix + 展開後)+tail
				char *res = ft_strjoin(tmp, tail);
				free(tmp);
				free(tail);
				if (!res) { free(tenkai_no_nakami); free(cur); return (1); }

				printf("saigo:%s\n", res);
				free(cur);
				cur = strdup(res);
				free(res);
				free(tenkai_no_nakami);
				if (!cur)
					return (1);
				j = saisyo + after;
				if (j < 0)
					j = 0;
				if (j > strlen(cur))
					j = strlen(cur);
				printf("saisyomoziretu:%s\n", cur);
			}
			else
				j = saisyo + 1;
		}
		av[i] = cur;
        i++;
	}
	return (0);
}

*/