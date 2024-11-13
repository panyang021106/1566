//
// Created by 潘阳 on 24-10-21.
//
#include <GL/glut.h>
#include "maze.h"
#include "camera.h"
#include "draw_maze.h"

// 定义全局变量
Maze maze;
Camera camera;

void display() {
    // 清空颜色和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 设置相机
    glLoadIdentity();
    gluLookAt(camera.eyeX, camera.eyeY, camera.eyeZ,
              camera.centerX, camera.centerY, camera.centerZ,
              0.0, 1.0, 0.0);

    // 绘制迷宫
    draw_maze(&maze);

    // 交换缓冲区，以显示绘制内容
    glutSwapBuffers();
}

void init() {
    // 设置背景颜色为黑色
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 初始化迷宫
    maze_generate(&maze);
    // 初始化相机
    camera_init(&camera, &maze);
}

int main(int argc, char** argv) {
    // 初始化 GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Maze");

    // 初始化设置
    init();

    // 注册显示回调函数
    glutDisplayFunc(display);

    // 进入 GLUT 主循环
    glutMainLoop();
    return 0;
}
