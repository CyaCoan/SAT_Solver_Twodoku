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
    // 头插法插入子句头结点
    ClauseNode *p_Clause = (ClauseNode *)malloc(sizeof(ClauseNode));
    p_Clause->literal_num = 1;
    p_Clause->next_clause = p_List->first_clause;
    p_List->first_clause = p_Clause;

    // 更新子句数
    p_List->clause_num++;

    // 添加单子句的文字
    p_Clause->first_literal = (LiteralNode *)malloc(sizeof(LiteralNode));
    p_Clause->first_literal->literal = v;
    p_Clause->first_literal->next_literal = NULL;

    // 更新文字频度表
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
int SelectLiteral_0(List *p_List)
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
int SelectLiteral_1(List *p_List)
{
    int most_literal = 0;
    int max_freq = 0;

    // 寻找频度最高的正文字
    for (int i = 1; i <= p_List->variable_num; i++){
        if (p_List->pos_freq[i] > max_freq){
            max_freq = p_List->pos_freq[i];
            most_literal = i;
        }
    }

    // 未找到正文字则寻找频度最高的负文字
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

double J(int n)
{
    return pow(2.0, (double)(-n));
}

int SelectLiteral_2(List *p_List)
{
    double *weight = (double *)malloc(sizeof(double) * (p_List->variable_num * 2 + 1));
    for (int i = 0; i <= p_List->variable_num * 2; i++){
        weight[i] = 0.0;
    }

    for (ClauseNode *p_Clause = p_List->first_clause; 
        p_Clause; p_Clause = p_Clause->next_clause){

        for (LiteralNode *p_Literal = p_Clause->first_literal; 
            p_Literal; p_Literal = p_Literal->next_literal){
            
            if (p_Literal->literal > 0){
                weight[p_Literal->literal] += J(p_Clause->literal_num);
            }else{
                weight[p_List->variable_num - p_Literal->literal] += J(p_Clause->literal_num);
            }
        }
    }

    double max_weight = 0.0;
    int max_literal = 0;
    for (int i = 1; i <= p_List->variable_num; i++){
        if (weight[i] + weight[i + p_List->variable_num] > max_weight){
            max_weight = weight[i] + weight[i + p_List->variable_num];
            max_literal = i;
        }
    }
    if (weight[max_literal] < weight[max_literal + p_List->variable_num]){
        max_literal = 0 - max_literal;
    }

    free(weight);
    return max_literal;
}

int SelectLiteral_3(List *p_List)
{
    int *mark = (int *)malloc(sizeof(int) * (p_List->variable_num + 1));
    double *weight = (double *)malloc(sizeof(double) * (p_List->variable_num * 2 + 1));
    for (int i = 0; i <= p_List->variable_num; i++){
        mark[i] = 0;
    }
    for (int i = 0; i <= p_List->variable_num * 2; i++){
        weight[i] = 0.0;
    }

    for (ClauseNode *p_Clause = p_List->first_clause; 
        p_Clause; p_Clause = p_Clause->next_clause){

        for (LiteralNode *p_Literal = p_Clause->first_literal; 
            p_Literal; p_Literal = p_Literal->next_literal){
            
            if (p_Literal->literal < 0){
                break;
            }else if (p_Literal->next_literal == NULL){

                for (LiteralNode *p_TempLiteral = p_Clause->first_literal; 
                    p_TempLiteral; p_TempLiteral = p_TempLiteral->next_literal){
                    
                    mark[p_TempLiteral->literal] = 1;
                }
                break;
            }
        }
    }

    for (ClauseNode *p_Clause = p_List->first_clause; 
        p_Clause; p_Clause = p_Clause->next_clause){

        for (LiteralNode *p_Literal = p_Clause->first_literal; 
            p_Literal; p_Literal = p_Literal->next_literal){
            
            if (mark[abs(p_Literal->literal)]){
                if (p_Literal->literal > 0){
                    weight[p_Literal->literal] += J(p_Clause->literal_num);
                }else{
                    weight[p_List->variable_num - p_Literal->literal] += J(p_Clause->literal_num);
                }
            }
        }
    }

    free(mark);
    double max_weight = 0.0;
    int max_literal = 0;
    for (int i = 1; i <= p_List->variable_num; i++){
        if (weight[i] + weight[i + p_List->variable_num] > max_weight){
            max_weight = weight[i] + weight[i + p_List->variable_num];
            max_literal = i;
        }
    }
    if (weight[max_literal] < weight[max_literal + p_List->variable_num]){
        max_literal = 0 - max_literal;
    }

    free(weight);
    if (max_literal == 0){
        max_literal = SelectLiteral_2(p_List);
    }

    return max_literal;
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

    // 删除全部文字
    while (p_Clause->first_literal != NULL){
        p_Clause->first_literal = p_Literal->next_literal;

        // 更新文字频度表
        if (p_Literal->literal > 0){
            p_List->pos_freq[p_Literal->literal]--;
        }else{
            p_List->neg_freq[0 - p_Literal->literal]--;
        }
        free(p_Literal);

        p_Literal = p_Clause->first_literal;
    }

    // 更新子句数
    p_List->clause_num--;

    // 删除子句头结点
    if (p_Clause == p_List->first_clause){ // 删除的是第一个头结点
        p_List->first_clause = p_Clause->next_clause;
        free(p_Clause);
        return NULL; // 返回空指针
    }else{ // 删除的是其他头结点
        ClauseNode *p_PrevClause = p_List->first_clause;
        while (p_PrevClause->next_clause != p_Clause){
            p_PrevClause = p_PrevClause->next_clause;
        }
        p_PrevClause->next_clause = p_Clause->next_clause;
        free(p_Clause);
        return p_PrevClause; // 返回前驱指针
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
    // 更新子句的文字数
    p_Clause->literal_num--;
    
    // 删除文字结点
    if (p_Literal == p_Clause->first_literal){ // 删除的是第一个结点
        p_Clause->first_literal = p_Literal->next_literal;
    }else{ // 删除的是其他结点
        LiteralNode *p_PrevLiteral = p_Clause->first_literal;
        while (p_PrevLiteral->next_literal != p_Literal){
            p_PrevLiteral = p_PrevLiteral->next_literal;
        }
        p_PrevLiteral->next_literal = p_Literal->next_literal;
    }

    // 更新文字频度表
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

    // 遍历子句
    while (p_Clause != NULL){
        p_Literal = p_Clause->first_literal;

        // 寻找与单子句或它的逆相同的文字
        while (p_Literal != NULL){

            // 出现与单子句相同的文字则删除文字所在子句
            if (p_Literal->literal == unit_clause){
                p_Clause = DeleteClause(p_List, p_Clause);
                break;
            }

            // 出现与单子句的逆相同的文字则删除该文字
            if (p_Literal->literal == 0 - unit_clause){
                DeleteLiteral(p_List, p_Clause, p_Literal);
                break;
            }

            p_Literal = p_Literal->next_literal;
        }

        if (p_Clause != NULL){ // 删除的不是第一个子句
            p_Clause = p_Clause->next_clause; // 遍历下一个子句
        }else{ // 删除的是第一个子句
            p_Clause = p_List->first_clause; // 遍历第一个子句
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
    // 复制参数
    List *p_TempList = (List *)malloc(sizeof(List));
    p_TempList->variable_num = p_List->variable_num;
    p_TempList->clause_num = p_List->clause_num;
    p_TempList->first_clause = NULL;
    p_TempList->flag = p_List->flag;

    // 复制正负文字频度表
    p_TempList->pos_freq = (int *)malloc(sizeof(int) * (p_TempList->variable_num + 1));
    p_TempList->neg_freq = (int *)malloc(sizeof(int) * (p_TempList->variable_num + 1));
    for (int i = 1; i <= p_TempList->variable_num; i++){
        p_TempList->pos_freq[i] = p_List->pos_freq[i];
        p_TempList->neg_freq[i] = p_List->neg_freq[i];
    }

    ClauseNode *p_Clause = p_List->first_clause;
    ClauseNode *p_TempListTail = NULL;

    // 复制表
    for (int i = 0; i < p_TempList->clause_num; i++){
        // 复制子句头结点
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

        // 复制子句
        for (int j = 0; j < p_TempClause->literal_num; j++){
            // 复制文字结点
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
    if (p_List->pos_freq != NULL && p_List->neg_freq != NULL){
        free(p_List->pos_freq);
        free(p_List->neg_freq);
    }
}

/**
 * @brief 单子句传播（不断寻找单子句并化简范式，直到没有单子句）
 * 
 * @param p_List 存储CNF的十字链表
 * @param p_truth_table 变元真值表
 * @param start 开始时间
 * @return SAT 为空范式（可满足）
 * @return UNSAT 有空子句（不可满足）
 * @return TLE 超时（立即返回）
 * @return NORMAL 化简完毕（继续选择文字）
 */
status UnitClausePropagation(List *p_List, bool **p_truth_table, clock_t start)
{
    int unit_clause = 0;

    // 检查是否超时
    clock_t now = clock();
    if ((int)(now - start) / CLOCKS_PER_SEC >= TIME_LIMIT){
        return TLE;
    }

    // 不断寻找单子句并化简，直到没有单子句
    while (unit_clause = GetUnitClause(p_List)){
        // 给单子句赋值
        if (unit_clause > 0){
            (*p_truth_table)[unit_clause] = true;
        }else if (unit_clause < 0){
            (*p_truth_table)[0 - unit_clause] = false;
        }

        // 利用单子句化简范式
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
 * @param start 开始时间
 * @return SAT 可满足
 * @return UNSAT 不可满足
 * @return TLE 超时
 */
status DPLL(char *path_out, List *p_List, bool **p_truth_table, int mode, clock_t start)
{   
    // 单子句传播
    status state = UnitClausePropagation(p_List, p_truth_table, start);
    if (state != NORMAL){
        return state;
    }

    // 不同的文字选取策略
    int v;
    if (mode == 0) v = SelectLiteral_0(p_List);
    if (mode == 1) v = SelectLiteral_1(p_List);
    if (mode == 2) v = SelectLiteral_2(p_List);
    if (mode == 3) v = SelectLiteral_3(p_List);
    // printf("%d\n", v);

    // 复制表，以免破坏原表
    List *p_TempList = CopyList(p_List);

    // 选取的文字取真
    status res1 = DPLL(path_out, MergeUnitClause(p_List, v), p_truth_table, mode, start);
    if (res1 == SAT){
        return SAT;
    }else if (res1 == TLE){
        return TLE;
    }

    // 选取的文字取假
    status res2 = DPLL(path_out, MergeUnitClause(p_TempList, 0 - v), p_truth_table, mode, start);
    ClearList(p_TempList);
    free(p_TempList);
    return res2;
}