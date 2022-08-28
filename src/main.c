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
    Twodoku T;
    bool *truth_table;
    
    int op = 1;
    while (op){
        system("cls");
        printf("\n\n");
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
            return 0;

            default:
            printf("Input Error!\n");
            getchar();getchar();
            break;
        }
    }
    
    return 0;
}