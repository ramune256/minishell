
#include "minishell.h"



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

// char *tenkai(char *line,char **ev,char * exit_status,int j,int *ev_len)
// {
//     int i = 1;
//     int cnt = 0;
//     int line_len = strlen(line);
//     if(line[j] == '?')
//         return (exit_status);
//     int ev_num = 0;
// 	line = ft_substr(line,j,line_len);
//     while(ev[ev_num])
//     {
// 		cnt = 0;
//         while(ev[ev_num][cnt] != '=')
//             cnt++;
//         *ev_len = cnt;
//         if(!strncmp(line,ev[ev_num],*ev_len))
//             return(ft_substr(ev[ev_num],cnt + 1,strlen(ev[ev_num])));
//         ev_num++;
//     }
//     return strdup("batu");
// }

// int main(int ac,char **av,char **ev)
// {
//     int i = 1;
// 	   int j = 0;
// 	int ev_len = 0;
// 	char *tenkai_no_nakami = NULL;
// 	int after = 0;
// 	int before = 0;
//     (void)ac;
//     while(av[i])
//     {
// 		printf("saisyomoziretu:%s\n",av[i]);
//         while(av[i][j] != '$' && av[i][j])
//             j++;
// 		// printf("%s\n",av[i]);
// 		// printf("%c\n",av[i][j]);
// 		int saisyo = j;
// 		if(av[i][j] == '$' && av[i][j])
// 		{
//         	j++;
// 			tenkai_no_nakami = tenkai(av[i],ev,"exit_status",j,&ev_len);
//         	printf("tenkai:%s\n",tenkai_no_nakami);
// 			before = ev_len;
// 			after = strlen(tenkai_no_nakami);
// 			while(after)
// 				(void)(after--,j++);
// 		}
// 		else
// 		{
//         	i++;
// 			j = 0;
// 			printf("owari\n");
// 			if(!av[i])
// 				break;
// 		}
// 		char *res = NULL;
// 		res = ft_substr(av[i],0,saisyo);//saisyo
// 		printf("saisyo:%s\n",res);
// 		res = ft_strjoin(res,tenkai_no_nakami);//nakami
// 		res = ft_strjoin(res,ft_substr(av[i],saisyo + before +1,strlen(av[i]) - before+1 + after));//saigo
// 		printf("saigo:%s\n",res);
// 		av[i] = strdup(res);
// 	}
//     return 0;
// }
