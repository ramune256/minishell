
#include "minishell.h"


/*
memo
t_alloc
exit_status(int);
char **ev_clone

t_cmd ast 
argv
*/

char	*tenkai(char *line, t_alloc *heap, int j, int *ev_len)
{
	int		cnt;
	int		ev_num;
	char	*tail;
	if (!line)
		return (NULL);
	if (line[j] == '?')
	{
		*ev_len = 1;
		return (ft_itoa(heap->exit_status));
	}
	tail = ft_substr(line, j, ft_strlen(line) - j);
	if (!tail)
		return (NULL);
	ev_num = 0;
	while (heap->ev_clone[ev_num])
	{
		cnt = 0;
		while (heap->ev_clone[ev_num][cnt] && heap->ev_clone[ev_num][cnt] != '=')
			cnt++;
		if (heap->ev_clone[ev_num][cnt] == '=')
		{
		    *ev_len = cnt;
		    if (!ft_strncmp(tail, heap->ev_clone[ev_num], cnt))
		    {
			    char *val = ft_substr(heap->ev_clone[ev_num],(cnt + 1),ft_strlen(heap->ev_clone[ev_num]) - (cnt + 1));
			    free(tail);
			    return (val);
		    }
        }
		ev_num++;
	}
	return (free(tail),NULL);
}

bool free_expander(char *cur,char *tenkai_no_nakami,char *tmp)
{
    if(cur)
        free(cur);
    if(tenkai_no_nakami)
        free(tenkai_no_nakami);
    if(tmp)
        free(tmp);
    return false;
}

bool check(t_cmd *ast,t_alloc *heap)
{
	int i = 1;
	while(ast->argv[i])
	{
		char *cur = ft_strdup(ast->argv[i]);
		if (!cur)
			return (false);
		size_t j = 0;
		while (1)
		{
			while (cur[j] && cur[j] != '$')
				j++;
			if (!cur[j])
			{
				printf("owari\n");
				break;
			}
			size_t saisyo = j;
			j++;
			int ev_len = 0;
			char *tenkai_no_nakami = tenkai(cur, heap, j, &ev_len);
			if(tenkai_no_nakami)
			{
				size_t before = ev_len;//変数の展開前の長さ
				size_t after = ft_strlen(tenkai_no_nakami);//変数の展開後の長さ	

				//prefix →　変数前の文字列
				char *prefix = ft_substr(cur, 0, saisyo);
				if (!prefix)
                	return (free_expander(cur,tenkai_no_nakami,NULL));
				//prefix＋展開後の変数
				char *tmp = ft_strjoin(prefix, tenkai_no_nakami);
				free(prefix);
				if (!tmp)
					return (free_expander(cur,tenkai_no_nakami,NULL));
				int tail_start = saisyo + 1 + before;
				int cur_len = ft_strlen(cur);
				int tail_len = 0;
				if (tail_start <= cur_len)
					tail_len = cur_len - tail_start;
				else
					tail_len = 0;

				char *tail = ft_substr(cur, (unsigned int)tail_start, tail_len);
				if (!tail)
				    return (free_expander(cur,tenkai_no_nakami,tmp));

				//res = tmp(prefix + 展開後)+tail
				char *res = ft_strjoin(tmp, tail);
				free(tmp);
				free(tail);
				if (!res)
					return (free_expander(cur,tenkai_no_nakami,NULL));

				ft_printf("saigo:%s\n", res);
				free(cur);
				cur = ft_strdup(res);
				free(res);
				free(tenkai_no_nakami);
				if (!cur)
					return (false);
				j = saisyo + after;
				if (j > ft_strlen(cur))
					j = ft_strlen(cur);
			}
			else
				j = saisyo + 1;
		}
		ast->argv[i] = cur;
		ft_printf("%s\n",ast->argv[i]);
        i++;
	}
	return (true);
}

void expander(t_cmd *ast, t_alloc *heap)
{
    if(ast == NULL)
        return;

    if(ast->type == NODE_EXEC)
	{
        if(check(ast,heap) == false)
		{
			cleanup(heap);
			free(ast);
		}
	}
    if(ast->left)
        expander(ast->left,heap);
    if(ast->right)
       expander(ast->right,heap);
}
