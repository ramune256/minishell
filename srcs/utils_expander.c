#include "minishell.h"

void	be_zero(int *s_flag, int *d_flag, size_t *j)
{
	*s_flag = 0;
	*d_flag = 0;
	*j = 0;
}

bool	input_argv(int *i, char *cur, t_cmd *ast)
{
	char	*tmp;

	tmp = NULL;
	tmp = count_quote(cur);
	free(cur);
	if (!tmp)
		return (false);
	free(ast->argv[*i]);
	ast->argv[*i] = tmp;
	(*i)++;
	return (true);
}

char	*update_cur(size_t j, char *tmp, char *tail, char *cur)
{
	char	*res;

	res = NULL;
	res = ft_strjoin(tmp, tail);
	(free(tmp), free(tail), free(cur));
	if (!res)
		return (NULL);
	cur = res;
	if (!cur)
		return (NULL);
	if (j > ft_strlen(cur))
		j = ft_strlen(cur);
	return (cur);
}
