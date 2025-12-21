
#include "minishell.h"

void flag_quote(size_t *j,int *s_flag,int *d_flag,char *cur)
{
	if(cur[*j] == '\"')
		(void)((*j)++,*d_flag = 1);
	if(cur[*j] && cur[*j] == '\'' && !*d_flag)
		(void)((*j)++,*s_flag = 1);
	while(cur[*j] && *s_flag && !*d_flag)
	{
		if(cur[*j] == '\'')
			*s_flag = 0;
		(*j)++;
	}
	while (cur[*j] && cur[*j] != '$')
		(*j)++;
}

char	*skip_quote(char *cur, char *new_cur, int i)
{
	int	cur_i;
	int	quote;

	cur_i = 0;
	quote = 0;
	while (cur[cur_i])
	{
		if (quote != 0 && cur[cur_i] == quote)
		{
			quote = 0;
			cur_i++;
		}
		else if (quote == 0 && (cur[cur_i] == '\'' || cur[cur_i] == '\"'))
		{
			quote = cur[cur_i];
			cur_i++;
		}
		else
		{
			new_cur[i] = cur[cur_i];
			(void)(i++, cur_i++);
		}
	}
	new_cur[i] = '\0';
	return (new_cur);
}

char	*count_quote(char *cur)
{
	int		cnt;
	int		q_cnt;
	char	*new_cur;

	cnt = 0;
	q_cnt = 0;
	new_cur = NULL;
	while (cur[cnt])
	{
		if (cur[cnt] == '\'' || cur[cnt] == '\"')
			q_cnt++;
		cnt++;
	}
	cnt++;
	if (!q_cnt)
		return (ft_strdup(cur));
	new_cur = malloc(sizeof(char) *(cnt - q_cnt + 1));
	if (!new_cur)
		return (NULL);
	return (skip_quote(cur, new_cur, 0));
}
