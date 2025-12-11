
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
