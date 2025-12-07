
#include "minishell.h"

char *input(char *old_str, int start, int len, char *val)
{
    int new_len = ft_strlen(old_str) - len + ft_strlen(val);
    char *new_str = malloc(sizeof(char) * (new_len + 1));
    
    if (!new_str)
        return (NULL);
    ft_strlcpy(new_str, old_str, start + 1);
    ft_strlcat(new_str, val, new_len + 1);
    ft_strlcat(new_str, old_str + start + len, new_len + 1);

    return (new_str);
}

char *tenkai(char *line,char **ev,int j,t_alloc *heap)
{
    while(j)
        (void)(line++,j--);
    line++;//$の分
    if(!ft_strncmp(line,"?",1))
        return (ft_itoa(heap->exit_status));
    int ev_num = 0;
    int cnt;
    while(ev[ev_num])
    {
        cnt = 0;
        while(ev[ev_num][cnt] != '=')
            cnt++;
        int ev_len =  cnt;
        if(!ft_strncmp(line, ev[ev_num], ev_len)
           && ev[ev_num][ev_len] == '='
           && ft_strncmp(ev[ev_num], "_", 1)
           && (line[ev_len] == '\0'
              || !(ft_isalnum(line[ev_len]) || line[ev_len] == '_')))
            return (ft_strdup(ev[ev_num] + ev_len + 1));
        ev_num++;
    }
    return (ft_strdup(""));
}

void doller_check(char **argv,t_alloc *heap)
{
    int i = 0;
    while (argv[i])
    {
        int j = 0;
        int single_flag = 0;
        while (argv[i][j])
        {
            if (argv[i][j] == '\'')
                single_flag = !single_flag;

            // シングルクォート外のみ処理
            if (argv[i][j] == '$' && !single_flag)
            {
                char *new = tenkai(argv[i], heap->ev_clone, j, heap);
                int len = 1 +ft_strlen(new);
                int before = ft_strlen(argv[i]);

                char *temp = input(argv[i], j, len, new);
                free(argv[i]);
                argv[i] = temp;

                int after = ft_strlen(argv[i]);
                j += (after - before);
                free(new);
            }
            j++;
        }
        i++;
    }
}

void check(t_cmd *ast,t_alloc *heap)
{
//   " ' をチェックしながら $探すチェック
    doller_check(ast->argv,heap);

      
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


/////////////////////////////

//展開のアジャイル開発用


// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	char	*substr;
// 	size_t	st_len;

// 	if (!s)
// 		return (NULL);
// 	if (len == 0)
// 		return (strdup(""));
// 	st_len = strlen(s);
// 	if (start >= st_len)
// 		return (strdup(""));
// 	if (len > st_len - start)
// 		len = st_len - start;
// 	substr = (char *)malloc(sizeof(char) * (len + 1));
// 	if (!substr)
// 		return (NULL);
// 	strlcpy(substr, s + start, len + 1);
// 	return (substr);
// }


// char *tenkai(char *line,char **ev,char * exit_status,int j)
// {
//     int i = 1;
//     int cnt = 0;
//     int line_len = strlen(line);
//     if(line[j] == '?')
//         return (exit_status);
//     int ev_num = 0;
// 	line = ft_substr(line,j,line_len);
// 	printf("%s\n",line);
//     while(ev[ev_num])
//     {
// 		cnt = 0;
//         while(ev[ev_num][cnt] != '=')
//             cnt++;
//         int ev_len = cnt;
//         if(!strncmp(line,ev[ev_num],ev_len))
//             return(ft_substr(ev[ev_num],cnt + 1,strlen(ev[ev_num])));
//         ev_num++;
//     }
//     return strdup("batu");
// }

// int main(int ac,char **av,char **ev)
// {
//     int i = 1;
// 	int j = 0;
//     (void)ac;
// 	printf("1\n");
//     while(av[i])
//     {
//         while(av[i][j] != '$' && av[i][j])
//             j++;
// 		// printf("%s\n",av[i]);
// 		// printf("%c\n",av[i][j]);
// 		if(av[i][j] == '$' && av[i][j])
// 		{
//         	j++;
//         	printf("%s\n",tenkai(av[i],ev,"exit_status",j));
// 			j++;
// 		}
// 		else
// 		{
//         	i++;
// 			j = 0;
// 			printf("3\n");
// 		}
//     }
//     return 0;
// }