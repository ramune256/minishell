
#include "minishell.h"

char *tenkai(char *line,char **ev)
{
    int i = 1;
    int line_len = ft_strlen(line);
    if(line[i] == '?')
        return (heap->exit_status);
    int ev_num = 0;
    while(ev[ev_num])
    {
        while(ev[ev_num][cnt] != '=')
            cnt++;
        int ev_len =  cnt;
        if(!ft_strncmp(line[i],ev[ev_num],ev_len))
            return(ft_strdup(ev[ev_num],ev_len));
        ev_num++;
    }
    else
        return NULL;
}

void doller_check(char **argv,t_alloc *heap)
{
    int i = 0;
    int j = 0;
    int single_flag = 0;
    while(argv[i])
    {
        if(argv[i][j] == "\'" || argv[i][j] == "\"")//'→フラグ
        {
            if(argv[i][j] == "\'")
                single_flag++;
            j++;
        }
        while(argv[i][j] != '$' || single_flag % 2)
            j++;
        if(argv[i][j] == '$')
        {
            char *new = NULL;
            new = tenkai(argv[i]);//新しく入れる文字を返す
            if(!new)
                //エラー時・heap全体をフリーとかするする
            input();//対象に新しい文字を入れて新しい配列にする &を使う
            int j_cnt = ft_strlen(new);//新しい文字増やした分jを増やす
            while(j_cnt)
                (void)(j++,j_cnt--);
        }
        if(single_flag % 2)
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
