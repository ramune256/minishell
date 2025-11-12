

#include "minishell.h"


//tokenizeでもし文字だったら環境変数かどうか調べる

//ドルがなかった場合そのまま返せるけどft_strdupで統一する？しなくてもいっか

char *expand_ev_value(t_alloc *heap,char *value,int exit_status)
{
    int i;
    int value_len;

    i = 0;
    if(value[i] != '$')
        return (value);
    value++;
    value_len = ft_strlen(value);
    if (!ft_strncmp(value,"?",2))
	    return (ft_itoa(exit_status));
    while(heap->ev[i])
    {
        if(!ft_strncmp(heap->ev[i],value,value_len))
        {
            if(heap->ev[i][value_len] == '=')
                return (ft_strdup(heap->ev[i] + value_len + 1),free(value));
        }
        i++;
    }
    return (ft_strdup(""),free(value));
}
