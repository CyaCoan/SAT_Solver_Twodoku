/**
 * @file Twodoku.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-25
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

void SetColorAndPrintNumber(int (*sudoku)[ROW][COL], int row, int col, int num)
{
    (*sudoku)[row][col] = 0;
    if (num < 0){
        if (CheckNumber(sudoku, row, col, 0 - num) == false){
            Color(BLACK + YELLOW * BACKGROUND);
        }else{
            Color(LIGHT_GREEN);
        }
        printf("%d", 0 - num);
        Color(WHITE);
        printf(" ");
    }else{
        if(num > 0){
            if (CheckNumber(sudoku, row, col, num) == false){
                Color(BLACK + LIGHT_RED * BACKGROUND);
            }else{
                Color(LIGHT_BLUE);
            }
        }else{
            Color(GREY);
        }
        printf("%d", num);
        Color(WHITE);
        printf(" ");
    }
    (*sudoku)[row][col] = num;
}

/**
 * @brief 打印双数独
 * 
 * @param twodoku 双数独
 */
void PrintTwodoku(Twodoku *twodoku)
{
    printf("The UL Sudoku\t\t");
    printf("The DR Sudoku\n");

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            SetColorAndPrintNumber(&twodoku->sudoku_UL, i, j, twodoku->sudoku_UL[i][j]);
            if (j == 2 || j == 5){
                printf(" ");
            }

        }

        printf("\t");

        for (int j = 0; j < 9; j++){
            SetColorAndPrintNumber(&twodoku->sudoku_DR, i, j, twodoku->sudoku_DR[i][j]);
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

Twodoku *CopyTwodoku(Twodoku *twodoku)
{
    Twodoku *temp_twodoku = (Twodoku *)malloc(sizeof(Twodoku));
    temp_twodoku->flag = twodoku->flag;
    temp_twodoku->clue_num = twodoku->clue_num;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            temp_twodoku->sudoku_UL[i][j] = twodoku->sudoku_UL[i][j];
            temp_twodoku->sudoku_DR[i][j] = twodoku->sudoku_DR[i][j];
            temp_twodoku->ans_UL[i][j] = twodoku->ans_UL[i][j];
            temp_twodoku->ans_DR[i][j] = twodoku->ans_DR[i][j];
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
        for (int i = 0; i < 9; i++){
            if (abs((*sudoku)[i][col]) == num || abs((*sudoku)[row][i]) == num){
                return false;
            }
        }
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
    if (pos == 81){
        return true;
    }

    int row = pos / 9;
    int col = pos % 9;
    if ((*sudoku)[row][col] != 0){
        return FillNumbers(sudoku, pos + 1, mode);
    }

    if (mode == 0){
        for (int i = 1; i <= 9; i++){
            if (CheckNumber(sudoku, row, col, i)){
                (*sudoku)[row][col] = i;
                if (FillNumbers(sudoku, pos + 1, mode)){
                    return true;
                }
                (*sudoku)[row][col] = 0;
            }
        }
    }

    if (mode == 1){
        for (int i = 9; i >= 1; i--){
            if (CheckNumber(sudoku, row, col, i)){
                (*sudoku)[row][col] = i;
                if (FillNumbers(sudoku, pos + 1, mode)){
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
            twodoku->ans_UL[i][j] = twodoku->sudoku_UL[i][j];
            twodoku->ans_DR[i][j] = twodoku->sudoku_DR[i][j];
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
bool DigHoles(Twodoku *twodoku, clock_t start)
{
    int hole_num = 35;

    SaveAnswer(twodoku);
    
    srand(time(NULL));

    for (int i = 0; i < hole_num; i++){
        if ((int)(clock() - start) * 1000 / CLOCKS_PER_SEC >= DIG_TIME_LIMIT){
            return false;
        }

        int pos = rand() % 81;
        int row = pos / 9;
        int col = pos % 9;

        if (twodoku->sudoku_UL[row][col] == 0){
            i--;
            continue;
        }

        int num = twodoku->sudoku_UL[row][col];
        twodoku->sudoku_UL[row][col] = 0;
        if (ConsistentWithAnswer(twodoku->sudoku_UL, twodoku->ans_UL) == false){
            twodoku->sudoku_UL[row][col] = num;
            i--;
        }
    }

    CopyOverlappedBlock(&twodoku->sudoku_UL, &twodoku->sudoku_DR);

    for (int i = 0; i < hole_num - 5; i++){
        if ((int)(clock() - start) * 1000 / CLOCKS_PER_SEC >= DIG_TIME_LIMIT){
            return false;
        }

        int pos = rand() % 81;
        int row = pos / 9;
        int col = pos % 9;

        if (twodoku->sudoku_DR[row][col] == 0 || (row < 3 && col < 3)){
            i--;
            continue;
        }

        int num = twodoku->sudoku_DR[row][col];
        twodoku->sudoku_DR[row][col] = 0;
        if (ConsistentWithAnswer(twodoku->sudoku_DR, twodoku->ans_DR) == false){
            twodoku->sudoku_DR[row][col] = num;
            i--;
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
            if (twodoku->sudoku_UL[i][j] != 0){
                twodoku->clue_num++;
            }
            if (twodoku->sudoku_DR[i][j] != 0){
                twodoku->clue_num++;
            }
        }
    }
}

/**
 * @brief 生成双数独初局
 * 
 * @param twodoku 双数独
 */
void GenerateTwodoku(Twodoku *twodoku)
{
    clock_t start = clock();

    SetZeros(&twodoku->sudoku_UL);
    SetZeros(&twodoku->sudoku_DR);

    for (int blockIndex = 0; blockIndex < 3; blockIndex++){
        SetDiagonalBlock(&twodoku->sudoku_UL, blockIndex, (unsigned int)blockIndex);
    }

    CopyOverlappedBlock(&twodoku->sudoku_UL, &twodoku->sudoku_DR);

    for (int blockIndex = 4; blockIndex < 6; blockIndex++){
        SetDiagonalBlock(&twodoku->sudoku_DR, blockIndex - 3, (unsigned int)blockIndex);
    }

    FillNumbers(&twodoku->sudoku_UL, 0, 0);
    FillNumbers(&twodoku->sudoku_DR, 0, 0);

    if (DigHoles(twodoku, clock()) == false){
        GenerateTwodoku(twodoku);
        return;
    }
    GetClueNumber(twodoku);

    PrintTwodoku(twodoku);

    clock_t end = clock();
    printf("time : %.2lfms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);

    twodoku->flag = true;
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
            if (twodoku->sudoku_UL[i - 1][j - 1] != 0){
                fprintf(p_File, "%d 0\n", (i - 1) * 81 + (j - 1) * 9 + twodoku->sudoku_UL[i - 1][j - 1]);
            }
        }
    }

    for (int i = 1; i <= 9; i++){
        for (int j = 1; j <= 9; j++){
            if (twodoku->sudoku_DR[i - 1][j - 1] != 0){
                fprintf(p_File, "%d 0\n", 729 + (i - 1) * 81 + (j - 1) * 9 + twodoku->sudoku_DR[i - 1][j - 1]);
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
    if (twodoku->flag == false){
        printf("The board is empty!\n");
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

    printf("The twodoku has been successfully transformed into a .cnf file called \"Twodoku.cnf\".\n");
    fclose(p_File);
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
bool CheckAnswer(List *p_List, bool **p_truth_table, Twodoku *twodoku)
{
    if (twodoku->flag == false){
        printf("The board is empty!\n");
        return ERROR;
    }else{
        if (*p_truth_table == NULL){
            printf("The solution is not found!\n");
            return ERROR;
        }
    }

    int solution[2][9][9];
    for (int i = 1; i <= p_List->variable_num; i++){
        if ((*p_truth_table)[i] == true){
            int board = (i - 1) / 729;
            int row = ((i - 1) % 729) / 81;
            int col = ((i - 1) % 81) / 9;
            int num = (i - 1) % 9 + 1;
            solution[board][row][col] = num;
        }
    }

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (solution[0][i][j] != twodoku->ans_UL[i][j] 
                || solution[1][i][j] != twodoku->ans_DR[i][j]){
                printf("Wrong answer!\n");
                return false;
            }
        }
    }

    Twodoku *temp_twodoku = CopyTwodoku(twodoku);

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (temp_twodoku->sudoku_UL[i][j] == 0){
                temp_twodoku->sudoku_UL[i][j] = 0 - solution[0][i][j];
            }
            if (temp_twodoku->sudoku_DR[i][j] == 0){
                temp_twodoku->sudoku_DR[i][j] = 0 - solution[1][i][j];
            }
        }
    }

    PrintTwodoku(temp_twodoku);
    printf("Correct answer!\n");

    free(temp_twodoku);
    return true;
}