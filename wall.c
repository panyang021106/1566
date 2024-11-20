//
// Created by 潘阳 on 24-11-19.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAZE_SIZE 15

typedef struct {
    int cells[MAZE_SIZE][MAZE_SIZE];
} Maze;

void carve_path(int row, int col, Maze* maze);
void maze_generate(Maze* maze);
void print_maze(const Maze* maze);

void carve_path(int row, int col, Maze* maze) {
    // 定义四个方向（上、右、下、左）
    int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    // 随机打乱方向顺序
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp[2] = {directions[i][0], directions[i][1]};
        directions[i][0] = directions[j][0];
        directions[i][1] = directions[j][1];
        directions[j][0] = temp[0];
        directions[j][1] = temp[1];
    }

    // 递归地进行路径雕刻
    for (int i = 0; i < 4; i++) {
        int new_row = row + directions[i][0] * 2;
        int new_col = col + directions[i][1] * 2;

        // 检查新的位置是否在迷宫范围内且为未雕刻区域
        if (new_row > 0 && new_row < MAZE_SIZE - 1 && new_col > 0 && new_col < MAZE_SIZE - 1 && maze->cells[new_row][new_col] == 1) {
            // 移除墙壁，创建路径
            maze->cells[row + directions[i][0]][col + directions[i][1]] = 0;
            maze->cells[new_row][new_col] = 0;
            carve_path(new_row, new_col, maze);
        }
    }
}

void maze_generate(Maze* maze) {
    // 初始化所有单元格为墙
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            maze->cells[i][j] = 1; // 初始化为普通墙
        }
    }

    // 随机数种子
    srand(time(NULL));

    // 从 (1, 1) 开始生成迷宫，确保四周有围墙
    maze->cells[1][1] = 0; // 设置初始点为通路
    carve_path(1, 1, maze);

    // 设置入口和出口
    maze->cells[1][0] = 0; // 左上角的入口
    maze->cells[MAZE_SIZE - 2][MAZE_SIZE - 1] = 0; // 右下角的出口

    // 重新设置特殊墙
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (((i % 2 == 1 && j % 2 == 0) || (i % 2 == 0 && j % 2 == 1)) && maze->cells[i][j] == 1) {
                maze->cells[i][j] = 2; // 将行或列只有一个是奇数的普通墙设置为特殊墙
            }
        }
    }
}

void print_maze(const Maze* maze) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (maze->cells[i][j] == 1) {
                printf("#"); // 普通墙
            } else if (maze->cells[i][j] == 2) {
                printf("@"); // 特殊墙
            } else {
                printf("x"); // 路径
            }
        }
        printf("\n");
    }
}

int main() {
    Maze maze;
    maze_generate(&maze);
    print_maze(&maze);
    return 0;
}


