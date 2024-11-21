//
// Created by 潘阳 on 24-11-19.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

#define MAZE_SIZE 7

typedef struct {
    int cells[MAZE_SIZE][MAZE_SIZE];
} Maze;

typedef struct {
    int row;
    int col;
} Point;

Point result_path[MAZE_SIZE * MAZE_SIZE];
int path_length = 0;

void carve_path(int row, int col, Maze* maze);
void maze_generate(Maze* maze);
void print_maze(const Maze* maze);
void solve_maze_bfs(Maze* maze, Maze* solution);
void print_solution(const Maze* maze);
void generate_instructions();

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
                printf(" "); // 路径
            }
        }
        printf("\n");
    }
}

void solve_maze_bfs(Maze* maze, Maze* solution) {
    // 初始化 solution 为 0
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            solution->cells[i][j] = 0;
        }
    }

    Point queue[MAZE_SIZE * MAZE_SIZE];
    int front = 0;
    int rear = 0;

    // 起始点
    queue[rear++] = (Point){1, 0}; // 储存当前节点位置
    solution->cells[1][0] = 3; // 标记为路径

    // 定义四个方向（上、右、下、左）
    int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    // 用于记录路径
    Point parent[MAZE_SIZE][MAZE_SIZE];
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            parent[i][j] = (Point){-1, -1};
        }
    }

    int min_steps[MAZE_SIZE][MAZE_SIZE];
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            min_steps[i][j] = INT_MAX;
        }
    }
    min_steps[1][0] = 0;

    while (front < rear) { // 未处理的节点
        Point current = queue[front++];
        int current_steps = min_steps[current.row][current.col];

        // 如果到达出口
        if (current.row == MAZE_SIZE - 2 && current.col == MAZE_SIZE - 1) {
            // 记录路径
            Point path[MAZE_SIZE * MAZE_SIZE];
            int local_path_length = 0;

            Point p = current;
            while (p.row != -1 && p.col != -1) {
                path[local_path_length++] = p;
                p = parent[p.row][p.col];
            }

            // 储存最短路径的坐标到全局数组 `result_path` 中
            path_length = local_path_length;
            for (int i = 0; i < path_length; i++) {
                result_path[i] = path[path_length - 1 - i];
            }

            // 打印最短路径的坐标（正向）
            printf("\n最短路径的坐标：\n");
            for (int i = 0; i < path_length; i++) {
                printf("(%d, %d)\n", result_path[i].row, result_path[i].col);
            }

            return;
        }

        // 尝试向四个方向移动
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + directions[i][0];
            int new_col = current.col + directions[i][1];

            // 检查新的位置是否在迷宫范围内且为通路且未访问过
            if (new_row >= 0 && new_row < MAZE_SIZE && new_col >= 0 && new_col < MAZE_SIZE &&
                maze->cells[new_row][new_col] == 0 && current_steps + 1 < min_steps[new_row][new_col]) {
                // 标记为路径并将新位置加入队列
                solution->cells[new_row][new_col] = 3;
                queue[rear++] = (Point){new_row, new_col};
                parent[new_row][new_col] = current;
                min_steps[new_row][new_col] = current_steps + 1;
            }
        }
    }
}

void print_solution(const Maze* maze) {
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (maze->cells[i][j] == 3) {
                printf("."); // 解决路径
            } else if (maze->cells[i][j] == 1) {
                printf("#"); // 普通墙
            } else if (maze->cells[i][j] == 2) {
                printf("@"); // 特殊墙
            } else {
                printf(" "); // 其他路径
            }
        }
        printf("\n");
    }
}

void generate_instructions() {
    if (path_length < 2) {
        printf("path too short\n");
        return;
    }

    printf("\nwalk instructions：\n");

    // 定义方向（上、右、下、左）
    int direction_codes[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int current_direction = -1;

    // 初始化第一个方向
    for (int i = 0; i < 4; i++) {
        if (result_path[1].row - result_path[0].row == direction_codes[i][0] &&
            result_path[1].col - result_path[0].col == direction_codes[i][1]) {
            current_direction = i;
            break;
            }
    }

    // 输出第一个方向的一步
    printf("go step\n");

    // 从第二步开始，逐步检查并输出每一步的指令
    for (int i = 2; i < path_length; i++) {
        int new_direction = -1;

        // 计算当前路径的方向
        for (int j = 0; j < 4; j++) {
            if (result_path[i].row - result_path[i - 1].row == direction_codes[j][0] &&
                result_path[i].col - result_path[i - 1].col == direction_codes[j][1]) {
                new_direction = j;
                break;
                }
        }

        if (new_direction == current_direction) {
            // 方向没有改变，输出一步
            printf("go step\n");
        } else {
            // 方向改变，输出转弯指令
            int turn_difference = (new_direction - current_direction + 4) % 4;
            if (turn_difference == 1) {
                printf("turn right\n");
            } else if (turn_difference == 3) {
                printf("turn left\n");
            } else if (turn_difference == 2) {
                printf("turn around\n");
            }

            // 更新方向并输出一步
            current_direction = new_direction;
            printf("go step\n");
        }
    }
}



int main() {
    Maze maze;
    // 使用一个自定义的函数来生成迷宫
    maze_generate(&maze);
    print_maze(&maze);

    Maze solution;
    solve_maze_bfs(&maze, &solution);
    printf("\n解决路径1:\n");
    print_solution(&solution);
    generate_instructions();

    return 0;
}


