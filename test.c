//
// Created by 潘阳 on 24-10-21.
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "maze.h"

// 打印迷宫的函数
void print_maze(const Maze* maze) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (maze->cells[i][j] == 1) {
                printf("# "); // 用 '#' 表示墙
            } else if (maze->cells[i][j] == 2) {
                printf("o "); // 用 'o' 表示访问过的路径
            } else {
                printf(". "); // 用 '.' 表示路径
            }
        }
        printf("\n");
    }
}

// 求解迷宫的函数，返回是否有解
int solve_maze(Maze* maze, int row, int col) {
    if (row < 0 || row >= MAZE_SIZE || col < 0 || col >= MAZE_SIZE) {
        return 0; // 超出边界
    }
    if (maze->cells[row][col] == 1 || maze->cells[row][col] == 2) {
        return 0; // 碰到墙或已访问过
    }
    if (row == MAZE_SIZE - 1 && col == MAZE_SIZE - 1) {
        maze->cells[row][col] = 2; // 标记出口
        return 1; // 找到出口
    }

    // 标记当前点为已访问
    maze->cells[row][col] = 2;

    // 尝试向四个方向移动
    if (solve_maze(maze, row + 1, col) || // 下
        solve_maze(maze, row - 1, col) || // 上
        solve_maze(maze, row, col + 1) || // 右
        solve_maze(maze, row, col - 1)) { // 左
        return 1;
        }

    // 如果没有路径，回溯
    maze->cells[row][col] = 0;
    return 0;
}

// 测试迷宫求解函数
void test_solve_maze(Maze* maze) {
    if (solve_maze(maze, 0, 0)) {
        printf("Maze is solvable.\n");
        printf("\nSolved Maze:\n");
        print_maze(maze);
    } else {
        printf("Maze is not solvable.\n");
    }
}

int main() {
    Maze maze;
    maze_generate(&maze);

    printf("Generated Maze:\n");
    print_maze(&maze);

    printf("\nTesting if the maze is solvable:\n");
    if (solve_maze(&maze, 0, 0)) {
        printf("\nSolved Maze:\n");
        print_maze(&maze);
    }

    else{
          printf("Maze is not solvable.\n");
    }
}




