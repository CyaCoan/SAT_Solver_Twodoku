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

/**
 * @brief 设置输出颜色
 * 
 * @param x 颜色对应参数
 */
void Color(short x)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

/**
 * @brief 获取输入的.cnf文件和输出的.res文件的路径
 * 
 * @param f_name 输入.cnf文件名
 * @param path_in 输入.cnf路径
 * @param path_out 输出.res路径
 * @param p_List 存储CNF的十字链表
 * 
 * @return true 获取成功
 * @return false 获取失败
 */
bool GetPaths(char *f_name, char **path_in, char **path_out, List *p_List)
{
    char *dir_in = "../test/";
    char *dir_out = "../res/";
    char *suffix = ".res";

    char file_name[FILE_NAME_SIZE];
    if (f_name == NULL){
        printf("    Please input your file name:\n");
        scanf("%s", &file_name);
    }else{
        strcpy(file_name, f_name);
    }

    // 获取输入路径（test文件夹路径加.cnf文件名）
    char *in = (char *)malloc(sizeof(char) * PATH_SIZE);
    strcpy(in, dir_in);
    strcat(in, file_name);

    // 检验输入路径是否合法
    FILE *fp = fopen(in, "r");
    if (fp == NULL){
        free(in);
        fclose(fp);
        printf("Invalid Path!\n");
        return false;
    }
    *path_in = in;
    fclose(fp);

    // 截取文件名无后缀部分
    *path_out = (char *)malloc(sizeof(char) * PATH_SIZE);
    int i = strlen(file_name) - 1;
    while (file_name[i] != '.') i--;
    file_name[i] = '\0';

    // 获取输出路径（res文件夹路径加.res文件名）
    p_List->name = (char *)malloc(sizeof(char) * (strlen(file_name) + 1));
    strcpy(p_List->name, file_name);
    strcpy(*path_out, dir_out);
    strcat(*path_out, file_name);
    strcat(*path_out, suffix);
    
    return true;
}

/**
 * @brief 解决SAT问题
 * 
 * @param path_out 输出.res文件路径
 * @param p_List 存储CNF的十字链表
 * @param p_truth_table 变元真值表
 * @param mode 模式（0为不优化，1为优化）
 */
void SATSolvingSystem(char *path_out, List *p_List, bool **p_truth_table)
{
    if (p_List->flag == false){
        printf("The .cnf file has not been read!\n");
        getchar();getchar();
        return;
    }

    int op = 1;
    while (op){
        system("cls");
        printf("                           Solve SAT\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.MAMS         2.Two-sided JW      3.Pos Two-sided JW\n");
        printf("          0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 3]:\n");
        scanf("%d", &op);

        if (op){
            DPLLSolveSAT(path_out, p_List, p_truth_table, op);
            getchar();getchar();
        }
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

    int op = 1;
    while (op){
        system("cls");
        printf("                            SAT Solver\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.Read CNF        2.Output CNF      3.Solve SAT\n");
        printf("          4.Check Answer    0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 4]:\n");
        scanf("%d", &op);
        switch (op){
            case 1:
            if (GetPaths(NULL, &path_in, &path_out, p_List)){
                ReadCNF(path_in, p_List);
            }
            getchar();getchar();
            break;

            case 2:
            OutputCNF(p_List);
            getchar();getchar();
            break;

            case 3:
            SATSolvingSystem(path_out, p_List, p_truth_table);
            break;

            case 4:
            CheckSATAnswer(p_List, p_truth_table);
            getchar();getchar();
            break;

            case 0:
            ClearList(p_List);
            free(p_truth_table);
            p_truth_table = NULL;
            break;

            default:
            printf("Input Error!\n");
            getchar();getchar();
            break;
        }
    }
}

/**
 * @brief 生成双数独初局系统
 * 
 * @param twodoku 双数独
 */
void TwodokuGeneratingSystem(Twodoku *twodoku, bool **p_truth_table, List *p_List, char **path_in, char **path_out)
{
    int op = 1;
    while (op){
        system("cls");
        printf("                           Generate TDK\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.Easy             2.Medium            3.Hard\n");
        printf("          4.Very Hard        0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 4]:\n");
        scanf("%d", &op);

        if (op){
            if (*p_truth_table != NULL){
                free(*p_truth_table);
                *p_truth_table = NULL;
            }
            GenerateTwodoku(twodoku, op);
            TwodokuToCNF(twodoku);
            GetPaths("Twodoku.cnf", path_in, path_out, p_List);
            ReadCNF(*path_in, p_List);
            OutputCNF(p_List);
            getchar();getchar();
        }
    }
}

/**
 * @brief 双数独游戏系统
 * 
 * @param twodoku 双数独
 */
void TwodokuPlayingSystem(Twodoku *twodoku)
{
    if (twodoku->flag_gen == false){
        printf("The twodoku has not been generated!\n");
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
    char *path_in;
    char *path_out;

    int op = 1;
    while (op){
        system("cls");
        printf("                             Twodoku\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.Generate TDK    2.Solve TDK       3.Check Answer\n");
        printf("          4.Play TDK        0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 4]:\n");
        scanf("%d", &op);
        switch (op){
            case 1:
            TwodokuGeneratingSystem(twodoku, p_truth_table, p_List, &path_in, &path_out);
            break;

            case 2:
            SATSolvingSystem(path_out, p_List, p_truth_table);
            break;

            case 3:
            CheckTwodokuAnswer(p_List, p_truth_table, twodoku);
            getchar();getchar();
            break;

            case 4:
            TwodokuPlayingSystem(twodoku);
            break;

            case 0:
            ClearList(p_List);
            twodoku->flag_cnf = false;
            free(twodoku->tdk);
            free(twodoku->ans);
            free(p_truth_table);
            p_truth_table = NULL;
            break;

            default:
            printf("Input Error!\n");
            getchar();getchar();
            break;
        }
    }
}