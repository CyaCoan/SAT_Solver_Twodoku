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
#include<string.h>
#include<time.h>

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
#define DIG_LIMIT 300

#define ROW 9
#define COL 9

#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define BLUE "\033[0;32;34m"
#define YELLOW "\033[1;33m"
#define LIGHT_CYAN "\033[1;36m"
#define DARK_GRAY "\033[1;30m"

typedef int status;

typedef struct List{
    bool flag;
    int clause_num;
    int variable_num;
    int *pos_freq;
    int *neg_freq;
    struct ClauseNode *first_clause;
} List;

typedef struct ClauseNode{
    int literal_num;
    struct ClauseNode *next_clause;
    struct LiteralNode *first_literal;
} ClauseNode;

typedef struct LiteralNode{
    int literal;
    struct LiteralNode *next_literal;
} LiteralNode;

typedef struct Twodoku{
    bool flag;
    int clue_num;
    int sudoku_UL[ROW][COL];
    int sudoku_DR[ROW][COL];
    int ans_UL[ROW][COL];
    int ans_DR[ROW][COL];
} Twodoku;

const int all_nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

clock_t start;
clock_t end;

#include"CNF_Parser.c"
#include"DPLL_Solver.c"
#include"Twodoku.c"
#include"Display.c"

#endif //_HEADER_H_