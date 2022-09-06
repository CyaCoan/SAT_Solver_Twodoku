/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include"header.h"

int main()
{
    List L;
    L.flag = false;

    Twodoku T;
    T.flag_gen = false;
    T.flag_cnf = false;
    T.tdk = (Board *)malloc(sizeof(Board));
    T.ans = (Board *)malloc(sizeof(Board));
    
    bool *truth_table = NULL;
    
    int op = 1;
    while (op){
        system("cls");
        printf("                               Menu\n");
        printf("-------------------------------------------------------------------\n");
        printf("          1.SAT Solver      2.Twodoku         0.Exit\n");
        printf("-------------------------------------------------------------------\n");
        printf("    Please choose your operation [0 ~ 2]:\n");
        scanf("%d", &op);
        switch (op){
            case 1:
            SATSolverSystem(&L, &truth_table);
            break;

            case 2:
            TwodokuSystem(&L, &truth_table, &T);
            break;

            case 0:
            break;

            default:
            printf("Input Error!\n");
            getchar();getchar();
            break;
        }
    }
    
    return 0;
}