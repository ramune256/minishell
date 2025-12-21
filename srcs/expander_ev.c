#include "minishell.h"

char	*main_chenge_ev(char **ev, int ev_num, size_t *ev_len, char *tail)
{
	size_t	cnt;
	char	*val;
	int		str_end;

	val = NULL;
	while (ev[ev_num])
	{
		cnt = 0;
		while (ev[ev_num][cnt] && ev[ev_num][cnt] != '=')
			cnt++;
		if (ev[ev_num][cnt] == '=')
		{
			*ev_len = cnt;
			if (!ft_strncmp(tail, ev[ev_num], cnt))
			{
				str_end = ft_strlen(ev[ev_num]) - (cnt + 1);
				val = ft_substr(ev[ev_num], (cnt + 1), str_end);
				return (free(tail), val);
			}
		}
		ev_num++;
	}
	return (free(tail), NULL);
}

char	*chenge_ev(char *line, t_alloc *heap, size_t *j, size_t *ev_len)
{
	char	*tail;

	if (!line)
		return (NULL);
	if (line[*j] == '?')
	{
		*ev_len = 1;
		return (ft_itoa(heap->exit_status));
	}
	tail = ft_substr(line, *j, ft_strlen(line) - *j);
	if (!tail)
		return (NULL);
	return (main_chenge_ev(heap->ev_clone, 0, ev_len, tail));
}