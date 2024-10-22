#ifndef MAZE_H
#define MAZE_H

#define MAZE_SIZE 8

typedef struct {
    int cells[MAZE_SIZE][MAZE_SIZE]; // 1 表示墙，0 表示路径
} Maze;

void maze_generate(Maze* maze);
void gen_maze(int begin_row, int end_row, int begin_column, int end_column, Maze* maze);

#endif
