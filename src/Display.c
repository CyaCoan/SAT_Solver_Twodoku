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

void SATSolverSystem(List *p_List, bool **p_truth_table)
{
    char *path_in;
    char *path_out;
    p_List->flag = false;

    int op = 1;
    while (op){
        system("cls");
        printf("\n\n");
        printf("                            SAT Solver\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.Read CNF        2.Output CNF      3.Solve SAT\n");
        printf("          4.Optimization    0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 4]:\n");
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
            SolveSAT(path_out, p_List, p_truth_table, 0);
            getchar();getchar();
            break;

            case 4:
            SolveSAT(path_out, p_List, p_truth_table, 1);
            getchar();getchar();
            break;

            case 0:
            ClearList(p_List);
            return;

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
void SolveTwodoku(List *p_List, bool **p_truth_table, Twodoku *twodoku)
{
    if (twodoku->flag == false){
        printf("The board is empty!\n");
        getchar();getchar();
        return;
    }

    int op = 1;
    while (op){
        system("cls");
        printf("\n\n");
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
            SolveSAT("../res/Twodoku.res", p_List, p_truth_table, 1);
            getchar();getchar();
            break;

            case 0:
            return;

            default:
            printf("Input Error!\n");
            getchar();getchar();
            break;
        }
    }
}

void TwodokuSystem(List *p_List, bool **p_truth_table, Twodoku *twodoku)
{
    p_List->flag = false;
    twodoku->flag = false;
    *p_truth_table = NULL;
    
    int op = 1;
    while (op){
        system("cls");
        printf("\n\n");
        printf("                             Twodoku\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.GenerateTDK     2.TDKToCNF        3.SolveTDK\n");
        printf("          4.CheckAnswer     0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 4]:\n");
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
            SolveTwodoku(p_List, p_truth_table, twodoku);
            break;

            case 4:
            CheckAnswer(p_List, p_truth_table, twodoku);
            getchar();getchar();
            break;

            case 0:
            ClearList(p_List);
            return;

            default:
            printf("Input Error!\n");
            getchar();getchar();
            break;
        }
    }
}