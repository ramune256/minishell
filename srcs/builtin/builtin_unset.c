/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:46:10 by nmasuda           #+#    #+#             */
/*   Updated: 2025/12/27 22:57:42 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool    is_unset_target(char *env_str, char **args)
{
    int        i;
    size_t    key_len;

    key_len = get_key_len(env_str, NULL);
    i = 1;
    while (args[i])
    {
        if (ft_strlen(args[i]) == key_len && \
            !ft_strncmp(env_str, args[i], key_len))
            return (true);
        i++;
    }
    return (false);
}

static int    recreate_env(char **args, t_alloc *heap, size_t new_size)
{
    char    **new_ev;
    int        i;
    int        j;

    new_ev = ft_calloc(new_size + 1, sizeof(char *));
    if (!new_ev)
        (cleanup(heap), exit(1));
    i = 0;
    j = 0;
    while (heap->ev_clone[i])
    {
        if (is_unset_target(heap->ev_clone[i], args) == false)
        {
            new_ev[j] = ft_strdup(heap->ev_clone[i]);
            if (!new_ev[j])
                (free_2d_array(&new_ev), cleanup(heap), exit(1));
            j++;
        }
        i++;
    }
    new_ev[j] = NULL;
    free_2d_array(&(heap->ev_clone));
    heap->ev_clone = new_ev;
    return (0);
}

int    c_unset(char **line, t_alloc *heap)
{
    int        i;
    size_t    count;

    if (!line || !heap->ev_clone)
        return (1);
    if (!line[OPT])
        return (0);
    count = 0;
    i = 0;
    while (heap->ev_clone[i])
    {
        if (is_unset_target(heap->ev_clone[i], line) == false)
            count++;
        i++;
    }
    return (recreate_env(line, heap, count));
}
