#include "minishell.h"

static void	append_new_env(char *arg, t_alloc *heap, char **new_ev, int i)
{
	char	*tmp_val;
	char	*tmp_key;
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos && arg[equal_pos - arg - 1] == '+')
	{
		tmp_key = ft_substr(arg, 0, equal_pos - arg - 1);
		if (!tmp_key)
			(free(new_ev), cleanup(heap), exit(1));
		tmp_val = ft_strdup(equal_pos);
		if (!tmp_val)
			(free(tmp_key), free(new_ev), cleanup(heap), exit(1));
		new_ev[i] = ft_strjoin(tmp_key, tmp_val);
		(free(tmp_key), free(tmp_val));
	}
	else
		new_ev[i] = ft_strdup(arg);
	if (!new_ev[i])
		(free(new_ev), cleanup(heap), exit(1));
	new_ev[i + 1] = NULL;
	free(heap->ev_clone);
	heap->ev_clone = new_ev;
}

void	append_ev(char *arg, int count, t_alloc *heap)
{
	char	**new_ev;
	int		i;

	new_ev = ft_calloc(count + 2, sizeof(char *));
	if (!new_ev)
		(cleanup(heap), exit(1));
	i = 0;
	while (heap->ev_clone[i])
	{
		new_ev[i] = heap->ev_clone[i];
		i++;
	}
	append_new_env(arg, heap, new_ev, i);
}

void	append_env_val(char *arg, t_alloc *heap, int i)
{
	char	*new_val;
	char	*tmp;
	char	*joined_key;

	new_val = ft_strdup(ft_strchr(arg, '=') + 1);
	if (!new_val)
		(cleanup(heap), exit(1));
	if (ft_strchr(heap->ev_clone[i], '='))
		tmp = ft_strjoin(heap->ev_clone[i], new_val);
	else
	{
		joined_key = ft_strjoin(heap->ev_clone[i], "=");
		if (!joined_key)
			(free(new_val), cleanup(heap), exit(1));
		tmp = ft_strjoin(joined_key, new_val);
		free(joined_key);
	}
	free(new_val);
	if (!tmp)
		(cleanup(heap), exit(1));
	free(heap->ev_clone[i]);
	heap->ev_clone[i] = tmp;
}
