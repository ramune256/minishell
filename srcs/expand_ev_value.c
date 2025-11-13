
#include "minishell.h"

// char *change_ev_value(t_alloc heap,char *value)//ダブルクォーテーションが来た時のドルマークが（連続で）来た時の対応
// {
// 	int i = 0;
// 	char **new_value = NULL;

// 	if(value[i] == "$")
// 		return(expand_ev_value(heap,value));
// 	else if(value[i] == "\"")
// 	{
// 		new_value = ft_split(value,' ');//空白でスプリット
// 		if(!new_value)
// 			return (NULL);
// 		while(new_value[i])
// 		{
// 			if(new_value[i] == "$")
// 			{
// 				new_value[i] = expand_ev_value(heap,value);
// 				if(!new_value)
// 					return (new_value_free(new_value),NULL);
// 			}
// 			i++;
// 		}
// 		return (rsplit(new_value));
// 	}
// 	else
// 		return (value);
// }

// char *rsplit(char **new_value)
// {
//     char *result = NULL;
//     char *tmp = NULL;
//     char *tmp_result = NULL;
//     int j = 0;

//     result = ft_strdup(new_value[j++]);
// 	if(!result)
// 		return (new_value_free(new_value),NULL);
//     while (new_value[j])
//     {
//         tmp = new_value[j];
//         tmp_result = ft_strjoin(result, tmp);
//         free(result);
// 		if(!tmp_result)
// 			return (new_value_free(new_value),NULL);
//         result = tmp_result; 
//         j++;
//     }
//     return (result);
// }

// void new_value_free(char **new_value)
// {
// 	int i = 0;
// 	while(new_value[i])
// 	{
// 		free(new_value[i])
// 		i++;
// 	}
// 	free(new_value);
// }


// "aaa bbb $HOME aaa $aaa"

char *expand_ev_value(t_alloc *heap,char *value)
{
	int i;
	int value_len;

	i = 0;

	value++;
	value_len = ft_strlen(value);
	if (!ft_strncmp(value,"?",2))
		return (ft_itoa(heap->exit_status));
	while(heap->ev[i])
    {
		if(!ft_strncmp(heap->ev[i],value,value_len))
		{
			if(heap->ev[i][value_len] == '=')
				return (free(value),ft_strdup(heap->ev[i] + value_len + 1));
		}
		i++;
	}
	return (free(value),ft_strdup(""));
}



cchar *expand_in_quote(t_alloc *heap, char *value)
{
    int     i;
    int     len;
    char    *result;
    char    *tmp;
    char    *expanded;
    char    var_name[256];
    int     j;
    char    one[2];

    i = 0;
    result = ft_strdup("");     // 最初は空文字列
    if (!result)
        return (NULL);

    len = ft_strlen(value);

    while (i < len)
    {
        if (value[i] == '$')
        {
            i++;

            /* case: "$" の直後が終端 → "$" を結果に追加して終了 */
            if (i >= len)
            {
                tmp = ft_strjoin(result, "$");
                free(result);
                return (tmp);
            }

            /* case: $? */
            if (value[i] == '?')
            {
                var_name[0] = '?';
                var_name[1] = '\0';
                i++;

                expanded = expand_ev_value(heap, var_name);
                if (!expanded)
                    return (free(result), NULL);

                tmp = ft_strjoin(result, expanded);
                free(expanded);
                free(result);
                result = tmp;
            }
            /* case: $VAR_NAME */
            else if (ft_isalpha(value[i]) || value[i] == '_')
            {
                j = 0;
                while (i < len && (ft_isalnum(value[i]) || value[i] == '_'))
                {
                    var_name[j] = value[i];
                    j++;
                    i++;
                }
                var_name[j] = '\0';

                expanded = expand_ev_value(heap, var_name);
                if (!expanded)
                    return (free(result), NULL);

                tmp = ft_strjoin(result, expanded);
                free(expanded);
                free(result);
                result = tmp;
            }
            /* case: 変数名ではない → "$" + 次の文字をそのまま追加 */
            else
            {
                /* "$" を追加 */
                tmp = ft_strjoin(result, "$");
                free(result);
                if (!tmp)
                    return (NULL);
                result = tmp;

                /* 次の1文字を追加 */
                one[0] = value[i];
                one[1] = '\0';

                tmp = ft_strjoin(result, one);
                free(result);
                if (!tmp)
                    return (NULL);
                result = tmp;

                i++;    // 次の文字に進む
            }
        }
        else
        {
            /* 通常文字1文字を result に追加 */
            one[0] = value[i];
            one[1] = '\0';
            i++;

            tmp = ft_strjoin(result, one);
            free(result);
            result = tmp;
        }
    }
    return (result);
}
