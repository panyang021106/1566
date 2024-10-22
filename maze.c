//
// Created by 潘阳 on 24-10-21.
//
#include "maze.h"
#include <stdlib.h>
#include <time.h>

void maze_generate(Maze* maze) {
    // 初始化所有单元格为空
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            maze->cells[i][j] = 0;
        }
    }

    // 随机数种子
    srand(time(NULL));

    // 从整个迷宫开始递归生成
    gen_maze(0, MAZE_SIZE - 1, 0, MAZE_SIZE - 1, maze);
}

void gen_maze(int begin_row, int end_row, int begin_column, int end_column, Maze* maze) {
    // 如果房间太小，停止递归
    if (end_row - begin_row < 2 || end_column - begin_column < 2) {
        return;
    }

    // 随机选择一个位置来划分房间
    int row = begin_row + (end_row - begin_row > 0 ? rand() % (end_row - begin_row) : 0);
    int column = begin_column + (end_column - begin_column > 0 ? rand() % (end_column - begin_column) : 0);

    // 创建四堵墙（北、东、南、西）
    for (int i = begin_column; i <= end_column; i++) {
        maze->cells[row][i] = 1; // 水平墙体
    }
    for (int i = begin_row; i <= end_row; i++) {
        maze->cells[i][column] = 1; // 垂直墙体
    }

    // 随机选择三堵墙打开入口
    int walls_to_open[4] = {0, 1, 2, 3};
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = walls_to_open[i];
        walls_to_open[i] = walls_to_open[j];
        walls_to_open[j] = temp;
    }

    for (int i = 0; i < 3; i++) {
        switch (walls_to_open[i]) {
            case 0: // 北墙
                if (column > begin_column) {
                    maze->cells[row][begin_column + rand() % (column - begin_column)] = 0;
                }
                break;
            case 1: // 东墙
                if (row > begin_row) {
                    maze->cells[begin_row + rand() % (row - begin_row)][column] = 0;
                }
                break;
            case 2: // 南墙
                if (column < end_column) {
                    maze->cells[row][column + 1 + rand() % (end_column - column)] = 0;
                }
                break;
            case 3: // 西墙
                if (row < end_row) {
                    maze->cells[row + 1 + rand() % (end_row - row)][column] = 0;
                }
                break;
        }
    }

    // 递归生成四个小房间
    gen_maze(begin_row, row - 1, begin_column, column - 1, maze);
    gen_maze(begin_row, row - 1, column + 1, end_column, maze);
    gen_maze(row + 1, end_row, begin_column, column - 1, maze);
    gen_maze(row + 1, end_row, column + 1, end_column, maze);
}

