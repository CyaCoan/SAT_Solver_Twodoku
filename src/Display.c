/**
 * @file Display.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

void Color(short x)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

/**
 * @brief 获取输入的.cnf文件和输出的.res文件的路径
 * 
 * @param path_in 输入.cnf路径
 * @param path_out 输出.res路径
 */
void GetPaths(char **path_in, char **path_out)
{
    char *dir_in = "../test/";
    char *dir_out = "../res/";
    char *suffix = ".res";

    char file_name[FILE_NAME_SIZE];
    printf("    Please input your file name:\n");
    scanf("%s", &file_name);

    *path_in = (char *)malloc(sizeof(char) * PATH_SIZE);
    strcpy(*path_in, dir_in);
    strcat(*path_in, file_name);

    *path_out = (char *)malloc(sizeof(char) * PATH_SIZE);
    int i = strlen(file_name) - 1;
    while (file_name[i] != '.') i--;
    file_name[i] = '\0';
    strcpy(*path_out, dir_out);
    strcat(*path_out, file_name);
    strcat(*path_out, suffix);
}

/**
 * @brief 解决SAT问题
 * 
 * @param path_out 输出.res文件路径
 * @param p_List 存储CNF的十字链表
 * @param p_truth_table 变元真值表
 * @param mode 模式（0为不优化，1为优化）
 */
void SolveSAT(char *path_out, List *p_List, bool **p_truth_table)
{
    if (p_List->flag == false){
        printf("The list is empty!\n");
        getchar();getchar();
        return;
    }

    int op = 1;
    while (op){
        system("cls");
        printf("                           Solve SAT\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.MAMS         2.two-sided JW      3.pos two-sided JW\n");
        printf("          0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 3]:\n");
        scanf("%d", &op);

        if (op == 0){
            break;
        }
        
        // 复制表，以免破坏原表
        List *p_TempList = CopyList(p_List);
        *p_truth_table = (bool *)malloc(sizeof(bool) * (p_List->variable_num + 1));

        // 记录DPLL算法所用时间
        clock_t start = clock();
        status res = DPLL(path_out, p_TempList, p_truth_table, op, start);
        clock_t end = clock();

        // 销毁副本
        ClearList(p_TempList);
        free(p_TempList);

        // 结果输出
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

            double time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
            printf("time : %.2lfms\n", time);
            fprintf(p_File, "t %.2lf\n", time);

        }

        fclose(p_File);

        printf("The result has been written in the .res file, which has the same name with the .cnf file.\n");
        printf("Please check the result in the folder \"res\".\n");
        getchar();getchar();
    }
}

/**
 * @brief SAT求解器系统
 * 
 * @param p_List 存储CNF的十字链表
 * @param p_truth_table 变元真值表
 */
void SATSolverSystem(List *p_List, bool **p_truth_table)
{
    char *path_in;
    char *path_out;
    p_List->flag = false;

    int op = 1;
    while (op){
        system("cls");
        printf("                            SAT Solver\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.Read CNF        2.Output CNF      3.Solve SAT\n");
        printf("          0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 3]:\n");
        scanf("%d", &op);
        switch (op){
            case 1:
            GetPaths(&path_in, &path_out);
            ReadCNF(path_in, p_List);
            getchar();getchar();
            break;

            case 2:
            OutputCNF(p_List);
            getchar();getchar();
            break;

            case 3:
            SolveSAT(path_out, p_List, p_truth_table);
            break;

            case 0:
            ClearList(p_List);
            break;

            default:
            printf("Input Error!\n");
            getchar();getchar();
            break;
        }
    }
}

/**
 * @brief 求解双数独
 * 
 * @param p_List 存储CNF的十字链表
 * @param p_truth_table 变元真值表
 * @param twodoku 双数独
 */
void TwodokuSolvingSystem(List *p_List, bool **p_truth_table, Twodoku *twodoku)
{
    if (twodoku->flag == false){
        printf("The board is empty!\n");
        getchar();getchar();
        return;
    }

    int op = 1;
    while (op){
        system("cls");
        printf("                             SolveTDK\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.Read CNF        2.Output CNF      3.Solve SAT\n");
        printf("          0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 3]:\n");
        scanf("%d", &op);
        switch (op){
            case 1:
            ReadCNF("../test/Twodoku.cnf", p_List);
            getchar();getchar();
            break;

            case 2:
            OutputCNF(p_List);
            getchar();getchar();
            break;

            case 3:
            SolveSAT("../res/Twodoku.res", p_List, p_truth_table);
            getchar();getchar();
            break;

            case 0:
            break;

            default:
            printf("Input Error!\n");
            getchar();getchar();
            break;
        }
    }
}

void PlayTwodoku(Twodoku *twodoku)
{
    int board = 1;
    int row;
    int col;
    int num;

    Twodoku *temp_twodoku = CopyTwodoku(twodoku);

    while (board){
        system("cls");
        PrintTwodoku(temp_twodoku);

        bool flag = true;
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++){
                if (abs(temp_twodoku->sudoku_UL[i][j]) != temp_twodoku->ans_UL[i][j] 
                    || abs(temp_twodoku->sudoku_DR[i][j]) != temp_twodoku->ans_DR[i][j]){
                    flag = false;
                }
            }
        }
        if (flag == true){
            printf("You win!\n");
            break;
        }

        printf("    Please input parameters:\n");
        scanf("%d", &board);
        if (board == 0){
            break;
        }
        scanf("%d %d %d", &row, &col, &num);

        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 0 || num > 9){
            printf("Invalid Input!\n");
            getchar();getchar();
            continue;
        }

        if (board == 1){
            if (temp_twodoku->sudoku_UL[row - 1][col - 1] > 0){
                printf("Clues can not be operated!\n");
                getchar();getchar();
            }else{
                temp_twodoku->sudoku_UL[row - 1][col - 1] = 0 - num;
                if (row >= 7 && row <= 9 && col >= 7 && col <= 9){
                    temp_twodoku->sudoku_DR[row - 7][col - 7] = temp_twodoku->sudoku_UL[row - 1][col - 1];
                }
            }
        }

        if (board == 2){
            if (temp_twodoku->sudoku_DR[row - 1][col - 1] > 0){
                printf("Clues can not be operated!\n");
                getchar();getchar();
            }else{
                temp_twodoku->sudoku_DR[row - 1][col - 1] = 0 - num;
                if (row >= 1 && row <= 3 && col >= 1 && col <= 3){
                    temp_twodoku->sudoku_UL[row + 5][col + 5] = temp_twodoku->sudoku_DR[row - 1][col - 1];
                }
            }
        }
    }

    free(temp_twodoku);
}

void TwodokuPlayingGuide()
{
    printf("If you want to fill in numbers, please input 4 parameters in order:\n\n");
    printf("<board>: 1 for the UL sudoku, 2 for the DR sudoku\n");
    printf("<row>: 1 to 9, for the row number of the grid\n");
    printf("<col>: 1 to 9, for the column number of the grid\n");
    printf("<num>: 0 to 9, for the number to be filled in\n\n");
    printf("Parameters should be separated by spaces\n\n");
    printf("If you want to exit, please input 0\n");
}

void TwodokuPlayingSystem(Twodoku *twodoku)
{
    if (twodoku->flag == false){
        printf("The board is empty!\n");
        getchar();getchar();
        return;
    }

    int op = 1;
    while (op){
        system("cls");
        printf("                             PlayTDK\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.Play            2.Guide           0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 2]:\n");
        printf("    (Please read the guide before playing)\n");
        scanf("%d", &op);
        switch (op){
            case 1:
            PlayTwodoku(twodoku);
            break;

            case 2:
            TwodokuPlayingGuide();
            getchar();getchar();
            break;

            case 0:
            break;
        }
    }
}

/**
 * @brief 双数独系统
 * 
 * @param p_List 存储CNF的十字链表
 * @param p_truth_table 变元真值表
 * @param twodoku 双数独
 */
void TwodokuSystem(List *p_List, bool **p_truth_table, Twodoku *twodoku)
{
    p_List->flag = false;
    twodoku->flag = false;
    *p_truth_table = NULL;
    
    int op = 1;
    while (op){
        system("cls");
        printf("                             Twodoku\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.GenerateTDK     2.TDKToCNF        3.SolveTDK\n");
        printf("          4.CheckAnswer     5.PlayTDK         0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 5]:\n");
        scanf("%d", &op);
        switch (op){
            case 1:
            GenerateTwodoku(twodoku);
            getchar();getchar();
            break;

            case 2:
            TwodokuToCNF(twodoku);
            getchar();getchar();
            break;

            case 3:
            TwodokuSolvingSystem(p_List, p_truth_table, twodoku);
            break;

            case 4:
            CheckAnswer(p_List, p_truth_table, twodoku);
            getchar();getchar();
            break;

            case 5:
            TwodokuPlayingSystem(twodoku);
            break;

            case 0:
            ClearList(p_List);
            break;

            default:
            printf("Input Error!\n");
            getchar();getchar();
            break;
        }
    }
}