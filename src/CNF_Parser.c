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
 * @brief 读取路径对应文件中的CNF并写入十字链表
 * 
 * @param path 输入.cnf路径
 * @param p_List 存储CNF的十字链表
 */
void ReadCNF(char *path, List *p_List)
{
    if (p_List->flag == true){
        printf("The list is not empty!\n");
        return;
    }
    
    FILE *p_File;
    if ( (p_File = fopen(path, "r")) == NULL ){
        printf("Can't open the file!\n");
        exit(-1);
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
    fscanf(p_File, "%s %d %d", &form, &p_List->variable_num, &p_List->clause_num);

    // 初始化正负文字频度表
    p_List->pos_freq = (int *)malloc(sizeof(int) * (p_List->variable_num + 1));
    p_List->neg_freq = (int *)malloc(sizeof(int) * (p_List->variable_num + 1));
    for (int i = 0; i <= p_List->variable_num; i++){
        p_List->pos_freq[i] = p_List->neg_freq[i] = 0;
    }

    // 建表
    p_List->first_clause = NULL;
    ClauseNode *p_ListTail = NULL;
    int literal;

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
        while ( fscanf(p_File, "%d", &literal) ){
            if (literal == 0){
                break;
            }

            // 更新子句的文字数和文字频度表
            p_Clause->literal_num++;

            if (literal > 0){
                p_List->pos_freq[literal]++;
            }else{
                p_List->neg_freq[0 - literal]++;
            }

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

    printf("Read Successfully!\n");
}

/**
 * @brief 将十字链表存储内容输出为文本以供查验
 * 
 * @param p_List 存储CNF的十字链表
 */
void OutputCNF(List *p_List)
{
    if (p_List->flag == false){
        printf("The list is empty!\n");
        return;
    }

    FILE *p_File;
    char *file_name = "CNF_Parser_Output.txt";
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

    printf("Output Successfully!\nPlease check the result in the file \"CNF_Parser_Output.txt\"\n");
}

/**
 * @brief 打印频度表
 * 
 * @param p_List 存储CNF的十字链表
 */
void PrintFrequency(List *p_List)
{
    for (int i = 1; i <= p_List->variable_num; i++){
        printf("%d ", p_List->pos_freq[i]);
    }

    printf("\n");
    
    for (int i = 1; i <= p_List->variable_num; i++){
        printf("%d ", p_List->neg_freq[i]);
    }
}