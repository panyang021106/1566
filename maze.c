//
// Created by 潘阳 on 24-10-21.
//
#include "maze.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
// 队列节点结构体
    typedef struct {
        int row;
        int col;
        int distance;
    } QueueNode;
        // 栈节点结构体
    typedef struct {
        int row;
        int col;
    } StackNode;

void carve_path(int row, int col, Maze* maze);

void maze_generate(Maze* maze) {
    // 初始化所有单元格为墙
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            maze->cells[i][j] = 1; // 1 表示墙
        }
    }

    // 随机数种子
    srand(time(NULL));

    // 从 (1, 1) 开始生成迷宫，确保四周有围墙
    carve_path(1, 1, maze);

    // 设置入口和出口
    maze->cells[1][0] = 0; // 左上角的入口
    maze->cells[MAZE_SIZE -1][1] = 0; // 出口

    // 确保倒数第二行有通道
    for (int col = 1; col < MAZE_SIZE - 1; col++) {
        if (maze->cells[MAZE_SIZE - 2][col] == 1) {
            maze->cells[MAZE_SIZE - 2][col] = 0; // 在倒数第二行挖开一部分路径
            break;
        }
    }
}


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

            // 递归雕刻新的路径
            carve_path(new_row, new_col, maze);
        }
    }
}

// 打印迷宫的函数
void print_maze(const Maze* maze) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (maze->cells[i][j] == 1) {
                printf("# "); // 用 '#' 表示墙
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
    if (row == MAZE_SIZE - 2 && col == MAZE_SIZE - 1) {
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

int solve_maze_bfs(Maze* maze, int start_row, int start_col, int end_row, int end_col) {
    // 定义四个方向（上、右、下、左）
    int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};



    // 创建队列
    QueueNode queue[MAZE_SIZE * MAZE_SIZE];
    int front = 0, rear = 0;

    // 标记访问过的位置
    bool visited[MAZE_SIZE][MAZE_SIZE] = {false};

    // 将起点加入队列
    queue[rear++] = (QueueNode){start_row, start_col, 0};
    visited[start_row][start_col] = true;

    // 开始 BFS
    while (front < rear) {
        QueueNode current = queue[front++];

        // 如果到达终点，返回距离
        if (current.row == end_row && current.col == end_col) {
            return current.distance;
        }

        // 扩展到四个方向
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + directions[i][0];
            int new_col = current.col + directions[i][1];

            // 检查是否在迷宫范围内并且是路径且没有访问过
            if (new_row >= 0 && new_row < MAZE_SIZE &&
                new_col >= 0 && new_col < MAZE_SIZE &&
                maze->cells[new_row][new_col] == 0 && !visited[new_row][new_col]) {

                // 标记为已访问并加入队列
                visited[new_row][new_col] = true;
                queue[rear++] = (QueueNode){new_row, new_col, current.distance + 1};
            }
        }
    }

    // 如果没有找到路径，返回 -1
    return -1;
}

// 使用显式栈的深度优先搜索 (DFS) 求解迷宫
int solve_maze_dfs_stack(Maze* maze, int start_row, int start_col, int end_row, int end_col) {
    // 定义四个方向（上、右、下、左）
    int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};



    // 创建栈
    StackNode stack[MAZE_SIZE * MAZE_SIZE];
    int top = -1;

    // 标记访问过的位置
    bool visited[MAZE_SIZE][MAZE_SIZE] = {false};

    // 将起点加入栈
    stack[++top] = (StackNode){start_row, start_col};
    visited[start_row][start_col] = true;

    // 开始 DFS
    while (top >= 0) {
        StackNode current = stack[top--];

        // 如果到达终点，返回 1 表示找到路径
        if (current.row == end_row && current.col == end_col) {
            return 1;
        }

        // 标记路径
        maze->cells[current.row][current.col] = 2;

        // 尝试向四个方向扩展
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + directions[i][0];
            int new_col = current.col + directions[i][1];

            // 检查是否在迷宫范围内并且是路径且没有访问过
            if (new_row >= 0 && new_row < MAZE_SIZE &&
                new_col >= 0 && new_col < MAZE_SIZE &&
                maze->cells[new_row][new_col] == 0 && !visited[new_row][new_col]) {

                // 标记为已访问并将其加入栈
                visited[new_row][new_col] = true;
                stack[++top] = (StackNode){new_row, new_col};
            }
        }
    }

    // 如果没有找到路径，返回 0
    return 0;
}





