
#include "minishell.h"

char *tenkai(char *line,char **ev,char * exit_status,int j,int *ev_len)
{
    int i = 1;
    int cnt = 0;
    int line_len = strlen(line);
    if(line[j] == '?')
        return (exit_status);
    int ev_num = 0;
	line = ft_substr(line,j,line_len);
    while(ev[ev_num])
    {
		cnt = 0;
        while(ev[ev_num][cnt] != '=')
            cnt++;
        *ev_len = cnt;
        if(!strncmp(line,ev[ev_num],*ev_len))
            return(ft_substr(ev[ev_num],cnt + 1,strlen(ev[ev_num])));
        ev_num++;
    }
    return strdup(" ");
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
