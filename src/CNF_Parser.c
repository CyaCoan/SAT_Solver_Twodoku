/**
 * @file CNF_Parser.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief 判断字符串是否为纯数字
 * 
 * @param str 字符串
 * @return true 是纯数字
 * @return false 不是纯数字
 */
bool IsPureNumber(char *str)
{
    for (int i = 0; i < strlen(str); i++){
        if (str[i] == '-' && i == 0){
            continue;
        }
        if (str[i] >= '0' && str[i] <= '9'){
            continue;
        }
        return false;
    }
    return true;
}

/**
 * @brief 读取路径对应文件中的CNF并写入十字链表
 * 
 * @param path 输入.cnf路径
 * @param p_List 存储CNF的十字链表
 */
void ReadCNF(char *path, List *p_List)
{
    FILE *p_File;
    if ( (p_File = fopen(path, "r")) == NULL ){
        printf("Can't open the file!\n");
        return;
    }

    // 读取.cnf文件注释
    char ch;
    char buff[BUFF_SIZE];
    ch = fgetc(p_File);
    while (ch != 'p'){
        fgets(buff, BUFF_SIZE, p_File);
        ch = fgetc(p_File);
    }

    // 读取参数
    char form[5];
    char var_num[20];
    char cls_num[20];
    fscanf(p_File, "%s %s %s", &form, &var_num, &cls_num);
    if (IsPureNumber(var_num) && IsPureNumber(cls_num) 
        && atoi(var_num) > 0 && atoi(cls_num) > 0){
        
        p_List->variable_num = atoi(var_num);
        p_List->clause_num = atoi(cls_num);
    }else{
        printf("There is a read error!\n");
        return;
    }

    // 建表
    p_List->first_clause = NULL;
    ClauseNode *p_ListTail = NULL;

    for (int i = 0; i < p_List->clause_num; i++){
        // 建子句头结点的链表
        ClauseNode *p_Clause = (ClauseNode *)malloc(sizeof(ClauseNode));
        LiteralNode *p_ClauseTail = NULL;
        p_Clause->literal_num = 0;

        if (p_List->first_clause == NULL){
            p_List->first_clause = p_Clause;
        }else{
            p_ListTail->next_clause = p_Clause;
        }
        p_ListTail = p_Clause;

        p_Clause->first_literal = NULL;
        
        // 建子句
        char ltrl[10];
        int literal;
        while ( fscanf(p_File, "%s", &ltrl) ){
            // 读取文字
            if (IsPureNumber(ltrl)){
                literal = atoi(ltrl);
            }else{
                printf("There is a read error!\n");
                ClearList(p_List);
                return;
            }

            if (literal == 0){
                break;
            }

            // 更新子句的文字数
            p_Clause->literal_num++;

            // 建文字节点的链表
            LiteralNode *p_Literal = (LiteralNode *)malloc(sizeof(LiteralNode));
            p_Literal->literal = literal;

            if (p_Clause->first_literal == NULL){
                p_Clause->first_literal = p_Literal;
            }else{
                p_ClauseTail->next_literal = p_Literal;
            }
            p_ClauseTail = p_Literal;

        }
        p_ClauseTail->next_literal = NULL;

    }
    p_ListTail->next_clause = NULL;

    p_List->flag = true;
    fclose(p_File);
}

/**
 * @brief 将十字链表存储内容输出为文本以供查验
 * 
 * @param p_List 存储CNF的十字链表
 */
void OutputCNF(List *p_List)
{
    if (p_List->flag == false){
        printf("The .cnf file has not been read!\n");
        return;
    }

    FILE *p_File;
    char *file_name = "../CNF_Parser_Output.txt";
    if ( (p_File = fopen(file_name, "w")) == NULL){
        printf("Can't open the file!\n");
        exit(-1);
    }
    
    ClauseNode *p_Clause = p_List->first_clause;
    LiteralNode *p_Literal = NULL;

    // 输出参数
    fprintf(p_File, "CNF Parser Output\n%d variable(s) and %d clause(s)\n", p_List->variable_num, p_List->clause_num);

    // 遍历子句
    while (p_Clause != NULL){
        p_Literal = p_Clause->first_literal;

        // 遍历文字
        while (p_Literal != NULL){
            fprintf(p_File, "%d ", p_Literal->literal);
            p_Literal = p_Literal->next_literal;
        }
        fprintf(p_File, "\n");

        p_Clause = p_Clause->next_clause;
    }

    fclose(p_File);
}