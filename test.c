//
// Created by 潘阳 on 24-10-21.
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "maze.h"

// 打印迷宫的函数
#include "maze.h"
#include <stdio.h>

int main() {
    Maze maze;
    maze_generate(&maze);

    printf("Generated Maze:\n");
    print_maze(&maze);

    printf("\nTesting if the maze is solvable(bfs):\n");
//    if (solve_maze(&maze, 0, 0)) {
    	if (solve_maze_bfs(&maze, 1, 0, MAZE_SIZE - 2, MAZE_SIZE - 1)){
        printf("\nSolved Maze bfs:\n");

        print_maze(&maze);
    } else {
        printf("Maze is not solvable.\n");
    }
    if (solve_maze_dfs_stack(&maze, 1, 0, MAZE_SIZE - 2, MAZE_SIZE - 1)){
        printf("\nSolved Maze Dfs:\n");

        print_maze(&maze);
    } else {
        printf("Maze is not solvable(DFS).\n");
    }
    return 0;
}





