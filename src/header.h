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

typedef struct List{                    // 表头结点
    char *name;                         // 表的名称
    bool flag;                          // 表的状态（空与不空）
    int clause_num;                     // 子句数
    int variable_num;                   // 变元数
    struct ClauseNode *first_clause;    // 第一个子句
} List;

typedef struct ClauseNode{              // 子句头结点
    int literal_num;                    // 文字数
    struct ClauseNode *next_clause;     // 下一个子句
    struct LiteralNode *first_literal;  // 第一个文字
} ClauseNode;

typedef struct LiteralNode{             // 文字结点
    int literal;                        // 文字
    struct LiteralNode *next_literal;   // 下一个文字
} LiteralNode;

typedef struct Board{                   // 双数独盘
    int sudoku_UL[ROW][COL];            // 左上数独
    int sudoku_DR[ROW][COL];            // 右下数独
} Board;

typedef struct Twodoku{                 // 双数独
    bool flag_gen;                      // 双数独的状态（是否生成了初局）
    bool flag_cnf;                      // 双数独的状态（是否转换为.cnf文件）
    int clue_num;                       // 提示数的个数
    Board *tdk;                         // 双数独的初局
    Board *ans;                         // 双数独的答案
} Twodoku;

const int all_nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
const int hole_nums[] = {0, 25, 35, 45, 55};

enum colors{
    BLACK, 
    BLUE, 
    GREEN, 
    LAKE_BLUE, 
    RED, 
    PURPLE, 
    YELLOW, 
    WHITE, 
    GREY, 
    LIGHT_BLUE, 
    LIGHT_GREEN, 
    LIGHT_CYAN, 
    LIGHT_RED, 
    LIGHT_PURPLE, 
    LIGHT_YELLOW, 
    LIGHT_WHITE, 
    BACKGROUND};

bool IsPureNumber(char *str);
void ReadCNF(char *path, List *p_List);
void OutputCNF(List *p_List);

int GetUnitClause(List *p_List);
bool IsEmptyCNF(List *p_List);
bool HaveEmptyClause(List *p_List);
List *MergeUnitClause(List *p_List, int v);
int SelectLiteral_1(List *p_List);
double GetWeight(int n);
int SelectLiteral_2(List *p_List);
int SelectLiteral_3(List *p_List);
ClauseNode *DeleteClause(List *p_List, ClauseNode *p_Clause);
void DeleteLiteral(List *p_List, ClauseNode *p_Clause, LiteralNode *p_Literal);
void SimplifyCNF(List *p_List, int unit_clause);
List *CopyList(List *p_List);
void ClearList(List *p_List);
status DPLLUnitPropagate(List *p_List, bool **p_truth_table, clock_t start);
status DPLL(char *path_out, List *p_List, bool **p_truth_table, int mode, clock_t start);
void DPLLSolveSAT(char *path_out, List *p_List, bool **p_truth_table, int mode);
bool EvaluateClause(ClauseNode *p_Clause, bool **p_truth_table);
bool CheckSATAnswer(List *p_List, bool **p_truth_table);

void SetZeros(int (*sudoku)[ROW][COL]);
void SetColorAndPrintNumber(int (*sudoku)[ROW][COL], int row, int col, int num);
void PrintSudoku(int (*sudoku)[ROW][COL]);
void PrintTwodoku(Board *twodoku);
Twodoku *CopyTwodoku(Twodoku *twodoku);
int *Shuffle(const int *arr, int len, unsigned int seed);
bool CheckNumber(int (*sudoku)[ROW][COL], int row, int col, int num);
void SetDiagonalBlock(int (*sudoku)[ROW][COL], int block_row, unsigned int block_seed);
void CopyOverlappedBlock(int (*sudoku_UL)[ROW][COL], int (*sudoku_DR)[ROW][COL]);
bool FillNumbers(int (*sudoku)[ROW][COL], int pos, int mode);
void SaveAnswer(Twodoku *twodoku);
bool ConsistentWithAnswer(int sudoku[ROW][COL], int ans[ROW][COL]);
bool DigHoles(Twodoku *twodoku, clock_t start, int hole_num);
void GetClueNumber(Twodoku *twodoku);
void GenerateTwodoku(Twodoku *twodoku, int diff);
void GridConstraint(FILE *p_File);
void RowConstraint(FILE *p_File);
void ColumnConstraint(FILE *p_File);
void BlockConstraint(FILE *p_File);
void OverlapConstraint(FILE *p_File);
void ClueConstraint(FILE *p_File, Twodoku *twodoku);
void TwodokuToCNF(Twodoku *twodoku);
bool CheckTwodokuAnswer(List *p_List, bool **p_truth_table, Twodoku *twodoku);
void PlayTwodoku(Twodoku *twodoku);
void TwodokuPlayingGuide();

void Color(short x);
void GetPaths(char *f_name, char **path_in, char **path_out, List *p_List);
void SATSolvingSystem(char *path_out, List *p_List, bool **p_truth_table);
void SATSolverSystem(List *p_List, bool **p_truth_table);
void TwodokuGeneratingSystem(Twodoku *twodoku, bool **p_truth_table);
void TwodokuPlayingSystem(Twodoku *twodoku);
void TwodokuSystem(List *p_List, bool **p_truth_table, Twodoku *twodoku);

#include"CNF_Parser.c"
#include"DPLL_Solver.c"
#include"Twodoku.c"
#include"Display.c"

#endif //_HEADER_H_