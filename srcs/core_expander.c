#include "minishell.h"

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

bool	prefix_add_after(int fir_before, int fir_after, char *tmp, char **cur)
{
	int		tail_start;
	int		cur_len;
	int		tail_len;
	char	*tail;

	tail = NULL;
	tail_start = fir_before + 1;
	cur_len = ft_strlen(*cur);
	tail_len = 0;
	if (tail_start <= cur_len)
		tail_len = cur_len - tail_start;
	else
		tail_len = 0;
	tail = ft_substr(*cur, (unsigned int)tail_start, tail_len);
	if (!tail)
		return (free(*cur), free(tmp), false);
	*cur = update_cur(fir_after, tmp, tail, *cur);
	if (!*cur)
		return (false);
	return (true);
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

bool	check(t_cmd *ast, t_alloc *heap)
{
	int		i;
	int		s_flag;
	int		d_flag;
	char	*cur;
	size_t	j;

	i = 1;
	cur = NULL;
	while (ast->argv[i])
	{
		cur = ft_strdup(ast->argv[i]);
		if (!cur)
			return (false);
		be_zero(&s_flag, &d_flag, &j);
		while (1)
		{
			flag_quote(&j, &s_flag, &d_flag, cur);
			if (!cur[j])
				break ;
			cur = create_cur(&j, cur, heap);
			if (!cur)
				return (false);
		}
		if (input_argv(&i, cur, ast) == false)
			return (false);
	}
	return (true);
}

void	expander(t_cmd *ast, t_alloc *heap)
{
	if (ast == NULL)
		return ;
	if (ast->type == NODE_EXEC)
	{
		if (check(ast, heap) == false)
		{
			cleanup(heap);
			free(ast);
		}
	}
	if (ast->left)
		expander(ast->left, heap);
	else if (ast->right)
		expander(ast->right, heap);
	else if (ast->subcmd)
		expander(ast->subcmd, heap);
}
