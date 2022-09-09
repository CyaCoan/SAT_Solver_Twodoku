/**
 * @file Twodoku.c
 * @author CyaCoan (cyacoan@163.com)
 * @brief 双数独游戏
 * @version 1.0
 * @date 2022-09-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief 将数独清零
 * 
 * @param sudoku 数独
 */
void SetZeros(int (*sudoku)[ROW][COL])
{
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            (*sudoku)[i][j] = 0;
        }
    }
}

/**
 * @brief 设定颜色并打印数字
 * 
 * @param sudoku 数独
 * @param row 行号
 * @param col 列号
 * @param num 数字
 */
void SetColorAndPrintNumber(int (*sudoku)[ROW][COL], int row, int col, int num)
{
    // 首先清除所在位置的数字，以便检查是否有数字冲突
    (*sudoku)[row][col] = 0;

    if (num < 0){ // 数字为负，代表填入的数
        if (CheckNumber(sudoku, row, col, 0 - num) == false){ // 有冲突，为黄字
            Color(YELLOW);
        }else{ // 无冲突，为浅绿字
            Color(LIGHT_GREEN);
        }
        printf("%d", 0 - num);
        Color(WHITE);printf(" ");
    }else{
        if(num > 0){ // 数字为正，代表原有的提示数
            if (CheckNumber(sudoku, row, col, num) == false){ // 有冲突，为红字
                Color(RED);
            }else{ // 无冲突，为亮白字
                Color(LIGHT_WHITE);
            }
        }else{ // 数字为零，代表空格，为灰字
            Color(GREY);
        }
        printf("%d", num);
        Color(WHITE);printf(" ");
    }

    // 将数字放回原位
    (*sudoku)[row][col] = num;
}

/**
 * @brief 打印双数独
 * 
 * @param twodoku 双数独本体
 */
void PrintTwodoku(Board *tdk)
{
    // 打印标题
    printf("\n");
    Color(LIGHT_PURPLE);printf("1");
    Color(WHITE);printf(" The UL Sudoku");printf("\t\t  ");
    Color(LIGHT_PURPLE);printf("2");
    Color(WHITE);printf(" The DR Sudoku");printf("\n");

    // 打印列号
    printf("  ");
    for (int i = 1; i <= 9; i++){
        Color(LIGHT_PURPLE);printf("%d", i);
        Color(WHITE);printf(" ");
        if (i == 3 || i == 6){
            printf(" ");
        }
    }
    printf("    ");

    printf("  ");
    for (int i = 1; i <= 9; i++){
        Color(LIGHT_PURPLE);printf("%d", i);
        Color(WHITE);printf(" ");
        if (i == 3 || i == 6){
            printf(" ");
        }
    }
    printf("\n");

    // 打印双数独本体
    for (int i = 0; i < 9; i++){
        // 打印行号
        Color(LIGHT_PURPLE);printf("%d", i + 1);
        Color(WHITE);printf(" ");
        for (int j = 0; j < 9; j++){
            SetColorAndPrintNumber(&tdk->sudoku_UL, i, j, tdk->sudoku_UL[i][j]);
            if (j == 2 || j == 5){
                printf(" ");
            }
        }
        printf("    ");

        // 打印行号
        Color(LIGHT_PURPLE);printf("%d", i + 1);
        Color(WHITE);printf(" ");
        for (int j = 0; j < 9; j++){
            SetColorAndPrintNumber(&tdk->sudoku_DR, i, j, tdk->sudoku_DR[i][j]);
            if (j == 2 || j == 5){
                printf(" ");
            }
        }
        printf("\n");

        if (i == 2 || i == 5 || i == 8){
            printf("\n");
        }
    }
    Color(WHITE);
}

/**
 * @brief 复制双数独
 * 
 * @param twodoku 双数独
 * @return Twodoku* 双数独的副本
 */
Twodoku *CopyTwodoku(Twodoku *twodoku)
{
    Twodoku *temp_twodoku = (Twodoku *)malloc(sizeof(Twodoku));
    temp_twodoku->flag_gen = twodoku->flag_gen;
    temp_twodoku->flag_cnf = twodoku->flag_cnf;
    temp_twodoku->clue_num = twodoku->clue_num;
    temp_twodoku->tdk = (Board *)malloc(sizeof(Board));
    temp_twodoku->ans = (Board *)malloc(sizeof(Board));
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            temp_twodoku->tdk->sudoku_UL[i][j] = twodoku->tdk->sudoku_UL[i][j];
            temp_twodoku->tdk->sudoku_DR[i][j] = twodoku->tdk->sudoku_DR[i][j];
            temp_twodoku->ans->sudoku_UL[i][j] = twodoku->ans->sudoku_UL[i][j];
            temp_twodoku->ans->sudoku_DR[i][j] = twodoku->ans->sudoku_DR[i][j];
        }
    }
    return temp_twodoku;
}

/**
 * @brief 打乱数字顺序
 * 
 * @param arr 原序列
 * @param len 序列长度
 * @param seed 随机数种子
 * @return int* 打乱后序列
 */
int *Shuffle(const int *arr, int len, unsigned int seed)
{
    int *temp_arr = (int *)malloc(sizeof(int) * len);

    for (int i = 0; i < len; i++){
        temp_arr[i] = arr[i];
    }

    // 因为此函数被调用的时间间隔太短，随机数种子未发生变化
    // 需要加上额外的种子以生成不同的随机数序列
    srand(time(NULL) + seed);
    for (int i = 0; i < len; i++){
        int j = rand() % len;
        int k = rand() % len;
        int temp = temp_arr[j];
        temp_arr[j] = temp_arr[k];
        temp_arr[k] = temp;
    }

    return temp_arr;
}

/**
 * @brief 检查所填数字是否符合数独规则
 * 
 * @param sudoku 数独
 * @param row 行号
 * @param col 列号
 * @param num 所填数字
 * @return true 符合规则
 * @return false 不符合规则
 */
bool CheckNumber(int (*sudoku)[ROW][COL], int row, int col, int num)
{
    if (num != 0){
        // 行列检查
        for (int i = 0; i < 9; i++){
            if (abs((*sudoku)[i][col]) == num || abs((*sudoku)[row][i]) == num){
                return false;
            }
        }
        // 宫检查
        int block_row = row / 3;
        int block_col = col / 3;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (abs((*sudoku)[block_row * 3 + i][block_col * 3 + j]) == num){
                    return false;
                }
            }
        }
    }
    return true;
}

/**
 * @brief 在对角线宫内填入乱序的数字1到9
 * 
 * @param block_row 宫的行号（列号）
 * @param block_seed 宫的随机数种子
 */
void SetDiagonalBlock(int (*sudoku)[ROW][COL], int block_row, unsigned int block_seed)
{
    int *nums = Shuffle(all_nums, 9, block_seed);
    int cnt = 0;
    
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            (*sudoku)[block_row * 3 + i][block_row * 3 + j] = nums[cnt++];
        }
    }
}

/**
 * @brief 将左上数独的第九宫复制给右下数独的第一宫
 * 
 * @param sudoku_UL 左上数独
 * @param sudoku_DR 右下数独
 */
void CopyOverlappedBlock(int (*sudoku_UL)[ROW][COL], int (*sudoku_DR)[ROW][COL])
{
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            (*sudoku_DR)[i][j] = (*sudoku_UL)[6 + i][6 + j];
        }
    }
}

/**
 * @brief 回溯法填充数独
 * 
 * @param sudoku 数独
 * @param pos 填数位置
 * @param mode 搜索模式（0为升序，1为降序）
 * @return true 填数成功
 * @return false 填数失败
 */
bool FillNumbers(int (*sudoku)[ROW][COL], int pos, int mode)
{
    // 搜索完毕，返回
    if (pos == 81){
        return true;
    }

    int row = pos / 9;
    int col = pos % 9;
    // 已经填数，跳过
    if ((*sudoku)[row][col] != 0){
        return FillNumbers(sudoku, pos + 1, mode);
    }

    // 升序搜索
    if (mode == 0){
        for (int i = 1; i <= 9; i++){
            if (CheckNumber(sudoku, row, col, i)){
                (*sudoku)[row][col] = i;
                if (FillNumbers(sudoku, pos + 1, mode) == true){
                    return true;
                }
                (*sudoku)[row][col] = 0;
            }
        }
    }

    // 降序搜索
    if (mode == 1){
        for (int i = 9; i >= 1; i--){
            if (CheckNumber(sudoku, row, col, i)){
                (*sudoku)[row][col] = i;
                if (FillNumbers(sudoku, pos + 1, mode) == true){
                    return true;
                }
                (*sudoku)[row][col] = 0;
            }
        }
    }

    return false;
}

/**
 * @brief 保存答案
 * 
 * @param twodoku 双数独
 */
void SaveAnswer(Twodoku *twodoku)
{
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            twodoku->ans->sudoku_UL[i][j] = twodoku->tdk->sudoku_UL[i][j];
            twodoku->ans->sudoku_DR[i][j] = twodoku->tdk->sudoku_DR[i][j];
        }
    }
}

/**
 * @brief 检查答案是否一致
 * 
 * @param sudoku 数独
 * @param ans 答案
 * @return true 一致
 * @return false 不一致
 */
bool ConsistentWithAnswer(int sudoku[ROW][COL], int ans[ROW][COL])
{
    int sud1[ROW][COL];
    int sud2[ROW][COL];

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            sud2[i][j] = sud1[i][j] = sudoku[i][j];
        }
    }

    // 从升序和降序两个方向搜索数独的答案
    // 如果数独有多解，则必有至少一个方向搜索出的答案与原答案不同
    // 此种检验方法可保证数独有唯一解
    FillNumbers(&sud1, 0, 0);
    FillNumbers(&sud2, 0, 1);

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (sud1[i][j] != ans[i][j] || sud2[i][j] != ans[i][j]){
                return false;
            }
        }
    }

    return true;
}

/**
 * @brief 挖洞法生成双数独初局
 * 
 * @param twodoku 双数独
 * @param start 开始时间
 * @return true 挖洞成功
 * @return false 挖洞超时
 */
bool DigHoles(Twodoku *twodoku, clock_t start, int hole_num)
{
    SaveAnswer(twodoku);
    
    srand(time(NULL));
    // 挖洞数的上下浮动
    hole_num += (rand() % 7 - 3);

    int hole_cnt = 0;
    for (int i = 0; i < hole_num; i++){
        // 挖洞超过时限，则挖洞失败
        if ((int)(clock() - start) * 1000 / CLOCKS_PER_SEC >= DIG_TIME_LIMIT){
            return false;
        }

        int pos = rand() % 81;
        int row = pos / 9;
        int col = pos % 9;

        // 已经挖洞的格子不能再挖
        if (twodoku->tdk->sudoku_UL[row][col] == 0){
            i--;
            continue;
        }
        
        int num = twodoku->tdk->sudoku_UL[row][col];
        twodoku->tdk->sudoku_UL[row][col] = 0;
        // 挖洞后有多解的格子不能挖
        if (ConsistentWithAnswer(twodoku->tdk->sudoku_UL, twodoku->ans->sudoku_UL) == false){
            twodoku->tdk->sudoku_UL[row][col] = num;
            i--;
        }

        // 记录左上数独第九宫的挖洞数
        if (row > 5 && col > 5){
            hole_cnt++;
        }
    }

    // 复制重复宫，之后不再另挖洞
    CopyOverlappedBlock(&twodoku->tdk->sudoku_UL, &twodoku->tdk->sudoku_DR);

    for (int i = 0; i < hole_num - hole_cnt; i++){
        // 挖洞超过时限，则挖洞失败
        if ((int)(clock() - start) * 1000 / CLOCKS_PER_SEC >= DIG_TIME_LIMIT){
            return false;
        }

        int pos = rand() % 81;
        int row = pos / 9;
        int col = pos % 9;

        // 已经挖洞的格子和右下数独第一宫的格子不能挖
        if (twodoku->tdk->sudoku_DR[row][col] == 0 || (row < 3 && col < 3)){
            i--;
            continue;
        }

        int num = twodoku->tdk->sudoku_DR[row][col];
        twodoku->tdk->sudoku_DR[row][col] = 0;
        // 挖洞后有多解的格子不能挖
        if (ConsistentWithAnswer(twodoku->tdk->sudoku_DR, twodoku->ans->sudoku_DR) == false){
            twodoku->tdk->sudoku_DR[row][col] = num;
            i--;
        }
    }

    // 改变不是提示数的数字的符号，方便双数独的格式化输出
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (twodoku->tdk->sudoku_UL[i][j] == 0){
                twodoku->ans->sudoku_UL[i][j] = 0 - twodoku->ans->sudoku_UL[i][j];
            }
            if (twodoku->tdk->sudoku_DR[i][j] == 0){
                twodoku->ans->sudoku_DR[i][j] = 0 - twodoku->ans->sudoku_DR[i][j];
            }
        }
    }

    return true;
}

/**
 * @brief 获取双数独提示数的个数
 * 
 * @param twodoku 双数独
 */
void GetClueNumber(Twodoku *twodoku)
{
    twodoku->clue_num = 0;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (twodoku->tdk->sudoku_UL[i][j] != 0){
                twodoku->clue_num++;
            }
            if (twodoku->tdk->sudoku_DR[i][j] != 0){
                twodoku->clue_num++;
            }
        }
    }
}

/**
 * @brief 生成双数独初局
 * 
 * @param twodoku 双数独
 * @param diff 难度
 */
void GenerateTwodoku(Twodoku *twodoku, int diff)
{
    clock_t start = clock();

    // 清零
    SetZeros(&twodoku->tdk->sudoku_UL);
    SetZeros(&twodoku->tdk->sudoku_DR);

    // 设置对角线宫的数字
    for (int blockIndex = 0; blockIndex < 3; blockIndex++){
        SetDiagonalBlock(&twodoku->tdk->sudoku_UL, blockIndex, (unsigned int)blockIndex);
    }

    CopyOverlappedBlock(&twodoku->tdk->sudoku_UL, &twodoku->tdk->sudoku_DR);

    for (int blockIndex = 4; blockIndex < 6; blockIndex++){
        SetDiagonalBlock(&twodoku->tdk->sudoku_DR, blockIndex - 3, (unsigned int)blockIndex);
    }

    // 填入剩余数字
    FillNumbers(&twodoku->tdk->sudoku_UL, 0, 0);
    FillNumbers(&twodoku->tdk->sudoku_DR, 0, 0);

    // 挖洞超时，则重新生成双数独
    if (DigHoles(twodoku, clock(), hole_nums[diff]) == false){
        GenerateTwodoku(twodoku, diff);
        return;
    }
    GetClueNumber(twodoku);

    // 打印双数独
    PrintTwodoku(twodoku->tdk);

    clock_t end = clock();
    // 输出生成耗时
    printf("time : %.2lfms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);

    twodoku->flag_gen = true;
    twodoku->flag_cnf = false;
}

/**
 * @brief 输出双数独格子约束条件
 * 
 * @param p_File 输出的.cnf文件
 */
void GridConstraint(FILE *p_File)
{
    if (p_File == NULL){
        exit(-1);
    }

    for (int grid = 1; grid <= 162; grid++){
        // 每个格子内填有数字1到9的其中一个
        for (int num = 1; num <= 9; num++){ // 对数字1到9
            fprintf(p_File, "%d ", (grid - 1) * 9 + num);
        }
        fprintf(p_File, "0\n");

        // 每个格子内只能填一个数字
        for (int num1 = 1; num1 <= 8; num1++){
            for (int num2 = num1 + 1; num2 <= 9; num2++){
                fprintf(p_File, "%d %d 0\n", -((grid - 1) * 9 + num1), -((grid - 1) * 9 + num2));
            }
        }
    }
}

/**
 * @brief 输出双数独行约束条件
 * 
 * @param p_File 输出的.cnf文件
 */
void RowConstraint(FILE *p_File)
{
    if (p_File == NULL){
        exit(-1);
    }

    for (int row = 1; row <= 18; row++){
        for (int num = 1; num <= 9; num++){ // 对数字1到9
            // 每行都有数字1到9
            for (int col = 1; col <= 9; col++){
                fprintf(p_File, "%d ", (row - 1) * 81 + (col - 1) * 9 + num);
            }
            fprintf(p_File, "0\n");

            // 每行中每个数字只能出现一次
            for (int col1 = 1; col1 <= 8; col1++){
                for (int col2 = col1 + 1; col2 <= 9; col2++){
                    fprintf(p_File, "%d %d 0\n", -((row - 1) * 81 + (col1 - 1) * 9 + num), 
                                                 -((row - 1) * 81 + (col2 - 1) * 9 + num));
                }
            }
        }
    }
}

/**
 * @brief 输出双数独列约束条件
 * 
 * @param p_File 输出的.cnf文件
 */
void ColumnConstraint(FILE *p_File)
{
    if (p_File == NULL){
        exit(-1);
    }

    for (int board = 1; board <= 2; board++){
        for (int col = 1; col <= 9; col++){
            for (int num = 1; num <= 9; num++){ // 对数字1到9
                // 每列都有数字1到9
                for (int row = 1; row <= 9; row++){
                    fprintf(p_File, "%d ", (board - 1) * 729 + (row - 1) * 81 + (col - 1) * 9 + num);
                }
                fprintf(p_File, "0\n");

                // 每列中每个数字只能出现一次
                for (int row1 = 1; row1 <= 8; row1++){
                    for (int row2 = row1 + 1; row2 <= 9; row2++){
                        fprintf(p_File, "%d %d 0\n", -((board - 1) * 729 + (row1 - 1) * 81 + (col - 1) * 9 + num), 
                                                     -((board - 1) * 729 + (row2 - 1) * 81 + (col - 1) * 9 + num));
                    }
                }
            }
        }
    }
}

/**
 * @brief 输出双数独宫约束条件
 * 
 * @param p_File 输出的.cnf文件
 */
void BlockConstraint(FILE *p_File)
{
    if (p_File == NULL){
        exit(-1);
    }

    for (int block = 1; block <= 18; block++){
        for (int num = 1; num <= 9; num++){ // 对数字1到9
            int vars[10];
            int cnt = 0;
            // 每宫都有数字1到9
            for (int row = ((block - 1) / 3) * 3 + 1; row <= ((block - 1) / 3) * 3 + 3; row++){
                for (int col = ((block - 1) % 3) * 3 + 1; col <= ((block - 1) % 3) * 3 + 3; col++){
                    fprintf(p_File, "%d ", (row - 1) * 81 + (col - 1) * 9 + num);
                    // 储存宫中变元
                    vars[++cnt] = (row - 1) * 81 + (col - 1) * 9 + num;
                }
            }
            fprintf(p_File, "0\n");

            // 每宫中每个数字只能出现一次
            for (int idx1 = 1; idx1 <= 8; idx1++){
                for (int idx2 = idx1 + 1; idx2 <= 9; idx2++){
                    fprintf(p_File, "%d %d 0\n", -vars[idx1], -vars[idx2]);
                }
            }
        }
    }
}

/**
 * @brief 输出双数独重叠约束条件
 * 
 * @param p_File 输出的.cnf文件
 */
void OverlapConstraint(FILE *p_File)
{
    if (p_File == NULL){
        exit(-1);
    }

    for (int num = 1; num <= 9; num++){ // 对数字1到9
        // 每个重叠的格子都是等价的
        for (int row = 7; row <= 9; row++){
            for (int col = 7; col <= 9; col++){
                fprintf(p_File, "%d %d 0\n", (row - 1) * 81 + (col - 1) * 9 + num, 
                                             -(729 + (row - 7) * 81 + (col - 7) * 9 + num));
                fprintf(p_File, "%d %d 0\n", -((row - 1) * 81 + (col - 1) * 9 + num), 
                                             729 + (row - 7) * 81 + (col - 7) * 9 + num);
            }
        }
    }
}

/**
 * @brief 输出双数独提示数约束条件
 * 
 * @param p_File 输出的.cnf文件
 * @param twodoku 双数独
 */
void ClueConstraint(FILE *p_File, Twodoku *twodoku)
{
    // 给出的提示数已经确定
    for (int i = 1; i <= 9; i++){
        for (int j = 1; j <= 9; j++){
            if (twodoku->tdk->sudoku_UL[i - 1][j - 1] != 0){
                fprintf(p_File, "%d 0\n", (i - 1) * 81 + (j - 1) * 9 + twodoku->tdk->sudoku_UL[i - 1][j - 1]);
            }
        }
    }

    for (int i = 1; i <= 9; i++){
        for (int j = 1; j <= 9; j++){
            if (twodoku->tdk->sudoku_DR[i - 1][j - 1] != 0){
                fprintf(p_File, "%d 0\n", 729 + (i - 1) * 81 + (j - 1) * 9 + twodoku->tdk->sudoku_DR[i - 1][j - 1]);
            }
        }
    }
}

/**
 * @brief 将双数独约束条件转化为.cnf文件
 * 
 * @param twodoku 双数独
 */
void TwodokuToCNF(Twodoku *twodoku)
{
    if (twodoku->flag_gen == false){
        printf("The twodoku has not been generated!\n");
        return;
    }

    FILE *p_File;
    if ((p_File = fopen("../test/Twodoku.cnf", "w")) == NULL){
        printf("Can't open the file!\n");
        exit(-1);
    }
    fprintf(p_File, "p cnf 1458 %d\n", 5994 * 4 + 162 + twodoku->clue_num);

    GridConstraint(p_File);
    RowConstraint(p_File);
    ColumnConstraint(p_File);
    BlockConstraint(p_File);
    OverlapConstraint(p_File);
    ClueConstraint(p_File, twodoku);

    fclose(p_File);

    twodoku->flag_cnf = true;
}

/**
 * @brief 检查SAT求解数独答案是否正确
 * 
 * @param p_List 存储CNF的十字链表（包括变元数量）
 * @param truth_table 变元真值表
 * @param twodoku 双数独
 * @return true 正确
 * @return false 错误
 */
bool CheckTwodokuAnswer(List *p_List, bool **p_truth_table, Twodoku *twodoku)
{
    if (twodoku->flag_gen == false){
        printf("The twodoku has not been generated!\n");
        return false;
    }else{
        if (*p_truth_table == NULL){
            printf("The solution has not been found!\n");
            return false;
        }
    }

    // 验证SAT求解结果
    if (CheckSATAnswer(p_List, p_truth_table) == false){
        return false;
    }

    PrintTwodoku(twodoku->ans);

    return true;
}

/**
 * @brief 双数独游戏
 * 
 * @param twodoku 双数独
 */
void PlayTwodoku(Twodoku *twodoku)
{
    int board = 1;
    int row;
    int col;
    int num;

    // 复制双数独，以免破坏原双数独
    Twodoku *temp_twodoku = CopyTwodoku(twodoku);

    while (board){
        system("cls");
        PrintTwodoku(temp_twodoku->tdk);

        // 判断胜利
        bool flag = true;
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++){
                if (abs(temp_twodoku->tdk->sudoku_UL[i][j]) != temp_twodoku->ans->sudoku_UL[i][j] 
                    || abs(temp_twodoku->tdk->sudoku_DR[i][j]) != temp_twodoku->ans->sudoku_UL[i][j]){
                    flag = false;
                }
            }
        }
        if (flag == true){
            printf("You win!\n");
            break;
        }

        // 接受输入参数并判断合法性
        printf("    Please input parameters:\n");
        scanf("%d", &board);
        if (board == 0){
            break;
        }
        if (board == -1){
            PrintTwodoku(twodoku->ans);
            getchar();getchar();
            continue;
        }

        scanf("%d %d %d", &row, &col, &num);

        if (board < 0 || board > 2 || row < 1 || row > 9 || col < 1 || col > 9 || num < 0 || num > 9){
            printf("Invalid Input!\n");
            getchar();getchar();
            continue;
        }

        // 左上数独
        if (board == 1){
            if (temp_twodoku->tdk->sudoku_UL[row - 1][col - 1] > 0){
                printf("Clues can not be edited!\n");
                getchar();getchar();
            }else{
                temp_twodoku->tdk->sudoku_UL[row - 1][col - 1] = 0 - num;
                if (row >= 7 && row <= 9 && col >= 7 && col <= 9){
                    temp_twodoku->tdk->sudoku_DR[row - 7][col - 7] = temp_twodoku->tdk->sudoku_UL[row - 1][col - 1];
                }
            }
        }

        // 右下数独
        if (board == 2){
            if (temp_twodoku->tdk->sudoku_DR[row - 1][col - 1] > 0){
                printf("Clues can not be edited!\n");
                getchar();getchar();
            }else{
                temp_twodoku->tdk->sudoku_DR[row - 1][col - 1] = 0 - num;
                if (row >= 1 && row <= 3 && col >= 1 && col <= 3){
                    temp_twodoku->tdk->sudoku_UL[row + 5][col + 5] = temp_twodoku->tdk->sudoku_DR[row - 1][col - 1];
                }
            }
        }
    }

    free(temp_twodoku);
}

/**
 * @brief 双数独游戏指南
 * 
 */
void TwodokuPlayingGuide()
{
    printf("INPUTS\n");
    printf("-------------------------------------------------------------------\n");
    printf("If you want to fill in numbers, please input four parameters in order:\n");
    printf("<board> <row> <col> <num>\n\n");
    printf("For example:\n");
    printf("\"1 2 3 4\" means filling in number 4 at the grid (row 2, col 3) of the upleft sudoku borad.\n");
    printf("\"2 8 9 0\" means clearing the number at the grid (row 8, col 9) of the downright sudoku board.\n\n");
    printf("If you want to exit, please input 0.\n\n");
    printf("If you want to get the answer, please input -1.\n\n");
    printf("OUTPUTS\n");
    printf("-------------------------------------------------------------------\n");
    Color(GREY);printf("0 Grey");Color(WHITE);printf(" - empty grids\n");
    Color(LIGHT_PURPLE);printf("1 Light purple");Color(WHITE);printf(" - labels of boards, rows and columns\n");
    Color(LIGHT_WHITE);printf("2 Light white");Color(WHITE);printf(" - uneditable clues\n");
    Color(LIGHT_GREEN);printf("3 Light green");Color(WHITE);printf(" - valid and editable filled-in numbers\n");
    Color(RED);printf("4 Red");Color(WHITE);printf(" - conflicted and uneditable numbers\n");
    Color(YELLOW);printf("5 Yellow");Color(WHITE);printf(" - conflicted and editable numbers\n");
}