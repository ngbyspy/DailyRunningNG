#pragma once
#ifndef CHECK_H
#define CHECK_H

// 定义结构体
typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} MyTriangle;

// 定义矩形结构体
typedef struct {
    int x, y;
    int width, height;
} MyRectangle;

// 声明函数原型
int checkRectangleCollision(MyRectangle rect1, MyRectangle rect2);
int checkTriangleRectangleCollision(MyTriangle triangle, MyRectangle rect);
int checkLineSegmentsIntersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
int direction(int x1, int y1, int x2, int y2, int x3, int y3);
int onSegment(int x1, int y1, int x2, int y2, int x, int y);
int minNum(int a, int b);
int maxNum(int a, int b);


#endif