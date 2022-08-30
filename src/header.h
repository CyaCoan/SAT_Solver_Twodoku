/**
 * @file header.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _HEADER_H_
#define _HEADER_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<windows.h>

#define POS 1
#define NEG -1

#define OK 1
#define ERROR 0

#define SAT 1
#define UNSAT 0
#define TLE -1
#define NORMAL -2

#define BUFF_SIZE 100
#define PATH_SIZE 70
#define FILE_NAME_SIZE 50
#define MAX 10000

#define TIME_LIMIT 10
#define DIG_TIME_LIMIT 300

#define ROW 9
#define COL 9

typedef int status;

typedef struct List{
    bool flag;                          // 表的状态（空与不空）
    int clause_num;                     // 子句数
    int variable_num;                   // 变元数
    int *pos_freq;                      // 正文字频度表
    int *neg_freq;                      // 负文字频度表
    struct ClauseNode *first_clause;    // 第一个子句
} List; // 表头结点

typedef struct ClauseNode{
    int literal_num;                    // 文字数
    struct ClauseNode *next_clause;     // 下一个子句
    struct LiteralNode *first_literal;  // 第一个文字
} ClauseNode; // 子句头结点

typedef struct LiteralNode{
    int literal;                        // 文字
    struct LiteralNode *next_literal;   // 下一个文字
} LiteralNode; // 文字结点

typedef struct Twodoku{
    bool flag;                          // 双数独的状态（是否生成了初局）
    int clue_num;                       // 提示数的个数
    int sudoku_UL[ROW][COL];            // 左上数独
    int sudoku_DR[ROW][COL];            // 右下数独
    int ans_UL[ROW][COL];               // 左上数独的答案
    int ans_DR[ROW][COL];               // 右下数独的答案
} Twodoku; // 双数独

const int all_nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

enum colors{BLACK, BLUE, GREEN, LAKE_BLUE, 
            RED, PURPLE, YELLOW, WHITE, 
            GREY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, 
            LIGHT_RED, LIGHT_PURPLE, LIGHT_YELLOW, LIGHT_WHITE, 
            BACKGROUND};

void SetZeros(int (*sudoku)[ROW][COL]);
void SetColorAndPrintNumber(int (*sudoku)[ROW][COL], int row, int col, int num);
void PrintSudoku(int (*sudoku)[ROW][COL]);
void PrintTwodoku(Twodoku *twodoku);
Twodoku *CopyTwodoku(Twodoku *twodoku);
int *Shuffle(const int *arr, int len, unsigned int seed);
bool CheckNumber(int (*sudoku)[ROW][COL], int row, int col, int num);
void SetDiagonalBlock(int (*sudoku)[ROW][COL], int block_row, unsigned int block_seed);
void CopyOverlappedBlock(int (*sudoku_UL)[ROW][COL], int (*sudoku_DR)[ROW][COL]);
bool FillNumbers(int (*sudoku)[ROW][COL], int pos, int mode);
void SaveAnswer(Twodoku *twodoku);
bool ConsistentWithAnswer(int sudoku[ROW][COL], int ans[ROW][COL]);
bool DigHoles(Twodoku *twodoku, clock_t start);
void GetClueNumber(Twodoku *twodoku);
void GenerateTwodoku(Twodoku *twodoku);
void GridConstraint(FILE *p_File);
void RowConstraint(FILE *p_File);
void ColumnConstraint(FILE *p_File);
void BlockConstraint(FILE *p_File);
void OverlapConstraint(FILE *p_File);
void ClueConstraint(FILE *p_File, Twodoku *twodoku);
void TwodokuToCNF(Twodoku *twodoku);
bool CheckAnswer(List *p_List, bool **p_truth_table, Twodoku *twodoku);

void Color(short x);
void GetPaths(char **path_in, char **path_out);
void SATSolverSystem(List *p_List, bool **p_truth_table);
void SolveTwodoku(List *p_List, bool **p_truth_table, Twodoku *twodoku);
void TwodokuSystem(List *p_List, bool **p_truth_table, Twodoku *twodoku);

#include"CNF_Parser.c"
#include"DPLL_Solver.c"
#include"Twodoku.c"
#include"Display.c"

#endif //_HEADER_H_