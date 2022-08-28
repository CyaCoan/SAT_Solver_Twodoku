/**
 * @file DPLL_Solver.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief 获取CNF中的单子句
 * 
 * @param p_List 存储CNF的十字链表
 * @return int 单子句对应的文字（获取成功）
 * @return 0（获取失败）
 */
int GetUnitClause(List *p_List)
{
    ClauseNode *p_Clause = p_List->first_clause;
    while (p_Clause != NULL){
        if (p_Clause->literal_num == 1){
            return p_Clause->first_literal->literal;
        }
        p_Clause = p_Clause->next_clause;
    }
    return 0;
}

/**
 * @brief 判断是否为空范式（无子句头结点）
 * 
 * @param p_List 存储CNF的十字链表
 * @return true 范式为空
 * @return false 范式非空
 */
bool IsEmptyCNF(List *p_List)
{
    if (p_List->clause_num == 0){
        return true;
    }
    return false;
}

/**
 * @brief 判断范式中是否有空子句（有子句头结点）
 * 
 * @param p_List 存储CNF的十字链表
 * @return true 有空子句
 * @return false 无空子句
 */
bool HaveEmptyClause(List *p_List)
{
    ClauseNode *p_Clause = p_List->first_clause;
    while (p_Clause != NULL){
        if (p_Clause->literal_num == 0){
            return true;
        }
        p_Clause = p_Clause->next_clause;
    }
    return false;
}

/**
 * @brief 将单子句加入范式中并更改频度表
 * 
 * @param p_List 存储CNF的十字链表
 * @param v 要加入的单子句
 * @return List* 加入单子句后的十字链表
 */
List *MergeUnitClause(List *p_List, int v)
{
    ClauseNode *p_Clause = (ClauseNode *)malloc(sizeof(ClauseNode));
    p_Clause->literal_num = 1;
    p_Clause->next_clause = p_List->first_clause;
    p_List->first_clause = p_Clause;
    p_List->clause_num++;

    p_Clause->first_literal = (LiteralNode *)malloc(sizeof(LiteralNode));
    p_Clause->first_literal->literal = v;
    p_Clause->first_literal->next_literal = NULL;

    if (v > 0){
        p_List->pos_freq[v]++;
    }else{
        p_List->neg_freq[0 - v]++;
    }

    return p_List;
}

/**
 * @brief 在没有单子句时选择一个文字作为单子句
 * 
 * @param p_List 存储CNF的十字链表
 * @return int 第一个文字（选择成功）
 * @return 0（选择失败）
 */
int SelectLiteral(List *p_List)
{
    ClauseNode *p_Clause = p_List->first_clause;
    while (p_Clause != NULL){
        if (p_Clause->literal_num != 0){
            return p_Clause->first_literal->literal;
        }
        p_Clause = p_Clause->next_clause;
    }
    return 0;
}

/**
 * @brief 在没有单子句时选择一个文字作为单子句（优化版）
 * 
 * @param p_List 存储CNF的十字链表（包含正负文字各自的频度表）
 * @return int 频度最高的文字（优先选择正文字）
 * @return 0（选择失败）
 */
int OptimizedSelectLiteral(List *p_List)
{
    int most_literal = 0;
    int max_freq = 0;

    for (int i = 1; i <= p_List->variable_num; i++){
        if (p_List->pos_freq[i] > max_freq){
            max_freq = p_List->pos_freq[i];
            most_literal = i;
        }
    }

    if (max_freq == 0){
        for (int i = 1; i <= p_List->variable_num; i++){
            if (p_List->neg_freq[i] > max_freq){
                max_freq = p_List->neg_freq[i];
                most_literal = 0 - i;
            }
        }
    }

    return most_literal;
}

/**
 * @brief 删除子句（包括子句头结点）并更改频度表
 * 
 * @param p_List 存储CNF的十字链表（包括频度表）
 * @param p_Clause 要删除的子句
 * @return ClauseNode* 要删除子句的上一个子句（方便循环删除）
 * @return NULL（当删除的是第一个子句）
 */
ClauseNode *DeleteClause(List *p_List, ClauseNode *p_Clause)
{
    LiteralNode *p_Literal = p_Clause->first_literal;
    while (p_Clause->first_literal != NULL){
        p_Clause->first_literal = p_Literal->next_literal;

        if (p_Literal->literal > 0){
            p_List->pos_freq[p_Literal->literal]--;
        }else{
            p_List->neg_freq[0 - p_Literal->literal]--;
        }
        free(p_Literal);

        p_Literal = p_Clause->first_literal;
    }

    p_List->clause_num--;

    if (p_Clause == p_List->first_clause){
        p_List->first_clause = p_Clause->next_clause;
        free(p_Clause);
        return NULL;
    }else{
        ClauseNode *p_PrevClause = p_List->first_clause;
        while (p_PrevClause->next_clause != p_Clause){
            p_PrevClause = p_PrevClause->next_clause;
        }
        p_PrevClause->next_clause = p_Clause->next_clause;
        free(p_Clause);
        return p_PrevClause;
    }
}

/**
 * @brief 删除文字并更改频度表
 * 
 * @param p_List 存储CNF的十字链表（包括频度表）
 * @param p_Clause 要删除的文字所在的子句
 * @param p_Literal 要删除的文字
 */
void DeleteLiteral(List *p_List, ClauseNode *p_Clause, LiteralNode *p_Literal)
{
    p_Clause->literal_num--;
    
    if (p_Literal == p_Clause->first_literal){
        p_Clause->first_literal = p_Literal->next_literal;
    }else{
        LiteralNode *p_PrevLiteral = p_Clause->first_literal;
        while (p_PrevLiteral->next_literal != p_Literal){
            p_PrevLiteral = p_PrevLiteral->next_literal;
        }
        p_PrevLiteral->next_literal = p_Literal->next_literal;
    }

    if (p_Literal->literal > 0){
        p_List->pos_freq[p_Literal->literal]--;
    }else{
        p_List->neg_freq[0 - p_Literal->literal]--;
    }
    free(p_Literal);
}

/**
 * @brief 利用单子句化简范式
 * 
 * @param p_List 存储CNF的十字链表
 * @param unit_clause 单子句
 */
void SimplifyCNF(List *p_List, int unit_clause)
{
    ClauseNode *p_Clause = p_List->first_clause;
    LiteralNode *p_Literal = NULL;
    while (p_Clause != NULL){
        p_Literal = p_Clause->first_literal;
        while (p_Literal != NULL){
            if (p_Literal->literal == unit_clause){
                p_Clause = DeleteClause(p_List, p_Clause);
                break;
            }
            if (p_Literal->literal == 0 - unit_clause){
                DeleteLiteral(p_List, p_Clause, p_Literal);
                break;
            }
            p_Literal = p_Literal->next_literal;
        }
        if (p_Clause != NULL){
            p_Clause = p_Clause->next_clause;
        }else{
            p_Clause = p_List->first_clause;
        }
    }
}

/**
 * @brief 复制十字链表（包括频度表）
 * 
 * @param p_List 存储CNF的十字链表（包括频度表）
 * @return List* 十字链表的副本
 */
List *CopyList(List *p_List)
{
    List *p_TempList = (List *)malloc(sizeof(List));
    p_TempList->variable_num = p_List->variable_num;
    p_TempList->clause_num = p_List->clause_num;
    p_TempList->first_clause = NULL;
    p_TempList->flag = p_List->flag;

    p_TempList->pos_freq = (int *)malloc(sizeof(int) * (p_TempList->variable_num + 1));
    p_TempList->neg_freq = (int *)malloc(sizeof(int) * (p_TempList->variable_num + 1));
    for (int i = 1; i <= p_TempList->variable_num; i++){
        p_TempList->pos_freq[i] = p_List->pos_freq[i];
        p_TempList->neg_freq[i] = p_List->neg_freq[i];
    }

    ClauseNode *p_Clause = p_List->first_clause;
    ClauseNode *p_TempListTail = NULL;

    for (int i = 0; i < p_TempList->clause_num; i++){
        ClauseNode *p_TempClause = (ClauseNode *)malloc(sizeof(ClauseNode));
        p_TempClause->literal_num = p_Clause->literal_num;
        p_TempClause->first_literal = NULL;

        if (p_TempList->first_clause == NULL){
            p_TempList->first_clause = p_TempClause;
        }else{
            p_TempListTail->next_clause = p_TempClause;
        }
        p_TempListTail = p_TempClause;

        LiteralNode *p_Literal = p_Clause->first_literal;
        LiteralNode *p_TempClauseTail = NULL;

        for (int j = 0; j < p_TempClause->literal_num; j++){
            LiteralNode *p_TempLiteral = (LiteralNode *)malloc(sizeof(LiteralNode));
            p_TempLiteral->literal = p_Literal->literal;

            if (p_TempClause->first_literal == NULL){
                p_TempClause->first_literal = p_TempLiteral;
            }else{
                p_TempClauseTail->next_literal = p_TempLiteral;
            }
            p_TempClauseTail = p_TempLiteral;

            p_Literal = p_Literal->next_literal;
        }
        p_TempClauseTail->next_literal = NULL;

        p_Clause = p_Clause->next_clause;
    }
    p_TempListTail->next_clause = NULL;

    return p_TempList;
}

/**
 * @brief 清空十字链表
 * 
 * @param p_List 存储CNF的十字链表
 */
void ClearList(List *p_List)
{
    if (p_List->flag == false){
        return;
    }

    for (int i = 0; i < p_List->clause_num; i++){
        DeleteClause(p_List, p_List->first_clause);
    }
    p_List->first_clause = NULL;
    p_List->clause_num = 0;
    p_List->flag = false;
    free(p_List->pos_freq);
    free(p_List->neg_freq);
}

/**
 * @brief 单子句传播（不断寻找单子句并化简范式，直到没有单子句）
 * 
 * @param p_List 存储CNF的十字链表
 * @param p_truth_table 变元真值表
 * @return SAT 为空范式（可满足）
 * @return UNSAT 有空子句（不可满足）
 * @return TLE 超时（立即返回）
 * @return NORMAL 化简完毕（继续选择文字）
 */
status UnitClausePropagation(List *p_List, bool **p_truth_table)
{
    int unit_clause = 0;

    clock_t now = clock();
    if ((int)(now - start) / CLOCKS_PER_SEC >= TIME_LIMIT){
        return TLE;
    }

    while (unit_clause = GetUnitClause(p_List)){
        if (unit_clause > 0){
            (*p_truth_table)[unit_clause] = true;
        }else if (unit_clause < 0){
            (*p_truth_table)[0 - unit_clause] = false;
        }

        SimplifyCNF(p_List, unit_clause);
        
        if (IsEmptyCNF(p_List)){
            return SAT;
        }else if (HaveEmptyClause(p_List)){
            return UNSAT;
        }
    }

    return NORMAL;
}

/**
 * @brief DPLL算法
 * 
 * @param path_out 输出.res文件路径
 * @param p_List 存储CNF的十字链表
 * @param p_truth_table 变元真值表
 * @param mode 模式（0为不优化，1为优化）
 * @return SAT 可满足
 * @return UNSAT 不可满足
 * @return TLE 超时
 */
status DPLL(char *path_out, List *p_List, bool **p_truth_table, int mode)
{   
    status state = UnitClausePropagation(p_List, p_truth_table);
    if (state != NORMAL){
        return state;
    }

    int v;
    if (mode == 0) v = SelectLiteral(p_List);
    if (mode == 1) v = OptimizedSelectLiteral(p_List);
    // printf("%d\n", v);

    List *p_TempList = CopyList(p_List);

    status res1 = DPLL(path_out, MergeUnitClause(p_List, v), p_truth_table, mode);
    if (res1 == SAT){
        return SAT;
    }else if (res1 == TLE){
        return TLE;
    }

    status res2 = DPLL(path_out, MergeUnitClause(p_TempList, 0 - v), p_truth_table, mode);
    ClearList(p_TempList);
    free(p_TempList);
    return res2;
}

/*
bool OptimizedDPLL(List *p_List, bool *truth_table)
{
    List *stack[MAX] = {NULL};
    List *lists[MAX] = {NULL};
    List *p_TempList = NULL;

    int state;
    int cnt = 0;
    int top = 0;
    int v[MAX];

    stack[top++] = p_List;
    while (top){
        while ((p_List = stack[top - 1]) && (state = UnitClausePropagation(p_List, truth_table)) == NORMAL){
            p_TempList = CopyList(p_List);
            lists[cnt] = p_TempList;

            v[cnt] = OptimizedSelectLiteral(p_List);
            p_List = MergeUnitClause(p_List, v[cnt]);

            stack[top++] = p_List;
            cnt++;
        }
        if (state == true){
            return true;
        }
        top--;
        cnt--;
        DestroyList(p_List);
        while (top){
            p_List = lists[cnt];
            top--;
            stack[top++] = MergeUnitClause(p_List, 0 - v[cnt]);
        }
    }
    return false;
}
*/

/**
 * @brief 解决SAT问题
 * 
 * @param path_out 输出.res文件路径
 * @param p_List 存储CNF的十字链表
 * @param p_truth_table 变元真值表
 * @param mode 模式（0为不优化，1为优化）
 */
void SolveSAT(char *path_out, List *p_List, bool **p_truth_table, int mode)
{
    if (p_List->flag == false){
        printf("The list is empty!\n");
        return;
    }
    
    start = clock();

    List *p_TempList = CopyList(p_List);
    *p_truth_table = (bool *)malloc(sizeof(bool) * (p_List->variable_num + 1));

    status res = DPLL(path_out, p_TempList, p_truth_table, mode);

    ClearList(p_TempList);
    free(p_TempList);

    FILE *p_File;
    if ((p_File = fopen(path_out, "w")) == NULL){
        printf("Can't open the file!\n");
        exit(-1);
    }

    if (res == TLE){

        fprintf(p_File, "s -1\nv \nt %.2lf\n", (double)TIME_LIMIT * 1000.0);
        printf("Time Limit Exceeded!\n");

    }else{

        if (res == SAT){
            fprintf(p_File, "s 1\nv ");
            for (int i = 1; i <= p_List->variable_num; i++){
                fprintf(p_File, "%d ", ((*p_truth_table)[i] ? 1 : -1) * i);
            }
            printf("Satisfiable!\n");
        }

        if (res == UNSAT){
            fprintf(p_File, "s 0\nv");
            printf("Unsatisfiable!\n");
        }

        fprintf(p_File, "\n");

        end = clock();
        double time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        printf("time : %.2lfms\n", time);
        fprintf(p_File, "t %.2lf\n", time);

    }

    fclose(p_File);

    printf("The result has been written in the .res file, which has the same name with the .cnf file.\n");
    printf("Please check the result in the folder \"res\".\n");
}