#pragma once

void test_move_engine();

void test_all_moves();
void test_sanity_x4();
void test_sanity_prime();
void test_scramble();
void test_inverse();

#define IDDFS                  0
#define IDA_Zero               1
#define IDA_Misplaced_Stickers 2
#define IDA_Orientation        3
#define IDA_Misplaced_Pieces   4
#define IDA_Combined           5

void test_solver(int n);
