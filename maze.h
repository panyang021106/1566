#ifndef MAZE_H
#define MAZE_H

#define MAZE_SIZE 15

typedef struct {
    int cells[MAZE_SIZE][MAZE_SIZE];
} Maze;

void maze_generate(Maze* maze);
void carve_path(int row, int col, Maze* maze);
void print_maze(const Maze* maze);
int solve_maze(Maze* maze, int row, int col);
int solve_maze_bfs(Maze* maze, int start_row, int start_col, int end_row, int end_col);
int solve_maze_dfs_stack(Maze* maze, int start_row, int start_col, int end_row, int end_col);
#endif // MAZE_H