#include "Check.h"
#include <cmath>
#include <iostream>

//以下代码用于检测一个三角形和一个矩形是否相交--------------------------------------------------------------------------------------------------------------------
// 定义三角形结构体


// 检测两个矩形是否碰撞
int checkRectangleCollision(MyRectangle rect1, MyRectangle rect2) {
    if (rect1.x < rect2.x + rect2.width &&
        rect1.x + rect1.width > rect2.x &&
        rect1.y < rect2.y + rect2.height &&
        rect1.y + rect1.height > rect2.y) {
        return 1; // 两个矩形碰撞
    }
    return 0; // 两个矩形不碰撞
}

// 检测三角形与矩形是否碰撞
int checkTriangleRectangleCollision(MyTriangle triangle, MyRectangle rect) {
    // 检测三角形的每一条边与矩形是否相交
    if (checkLineSegmentsIntersect(triangle.x1, triangle.y1, triangle.x2, triangle.y2, rect.x, rect.y, rect.x + rect.width, rect.y) ||
        checkLineSegmentsIntersect(triangle.x2, triangle.y2, triangle.x3, triangle.y3, rect.x, rect.y, rect.x + rect.width, rect.y) ||
        checkLineSegmentsIntersect(triangle.x3, triangle.y3, triangle.x1, triangle.y1, rect.x, rect.y, rect.x + rect.width, rect.y) ||
        checkLineSegmentsIntersect(triangle.x1, triangle.y1, triangle.x2, triangle.y2, rect.x, rect.y + rect.height, rect.x + rect.width, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x2, triangle.y2, triangle.x3, triangle.y3, rect.x, rect.y + rect.height, rect.x + rect.width, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x3, triangle.y3, triangle.x1, triangle.y1, rect.x, rect.y + rect.height, rect.x + rect.width, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x1, triangle.y1, triangle.x2, triangle.y2, rect.x, rect.y, rect.x, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x2, triangle.y2, triangle.x3, triangle.y3, rect.x, rect.y, rect.x, rect.y + rect.height) ||
        checkLineSegmentsIntersect(triangle.x3, triangle.y3, triangle.x1, triangle.y1, rect.x, rect.y, rect.x, rect.y + rect.height)) {
        return 1; // 三角形与矩形相交
    }
    return 0; // 三角形与矩形不相交
}

// 检测两条线段是否相交
int checkLineSegmentsIntersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int d1 = direction(x3, y3, x4, y4, x1, y1);
    int d2 = direction(x3, y3, x4, y4, x2, y2);
    int d3 = direction(x1, y1, x2, y2, x3, y3);
    int d4 = direction(x1, y1, x2, y2, x4, y4);
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return 1; // 两条线段相交
    }
    else if (d1 == 0 && onSegment(x3, y3, x4, y4, x1, y1)) {
        return 1; // 两条线段共线且有重叠部分
    }
    else if (d2 == 0 && onSegment(x3, y3, x4, y4, x2, y2)) {
        return 1; // 两条线段共线且有重叠部分
    }
    else if (d3 == 0 && onSegment(x1, y1, x2, y2, x3, y3)) {
        return 1; // 两条线段共线且有重叠部分
    }
    else if (d4 == 0 && onSegment(x1, y1, x2, y2, x4, y4)) {
        return 1; // 两条线段共线且有重叠部分
    }
    return 0; // 两条线段不相交
}

// 计算向量的方向
int direction(int x1, int y1, int x2, int y2, int x3, int y3) {
    return ((x2 - x1) * (y3 - y1)) - ((x3 - x1) * (y2 - y1));
}

// 判断点是否在线段上
int onSegment(int x1, int y1, int x2, int y2, int x, int y) {
    if (x >= minNum(x1, x2) && x <= maxNum(x1, x2) && y >= minNum(y1, y2) && y <= maxNum(y1, y2)) {
        return 1; // 点在线段上
    }
    return 0; // 点不在线段上
}

// 获取两个数中的最小值
int minNum(int a, int b) {
    return a < b ? a : b;
}

// 获取两个数中的最大值
int maxNum(int a, int b) {
    return a > b ? a : b;
}

//以下代码用于检测两个椭圆是否相交--------------------------------------------------------------------------------------------------------------------

// 定义椭圆结构体
//struct Ellipse {
//    float x, y;     // 椭圆中心坐标
//    float width, height;   // 椭圆的宽度和高度
//};
//
//// 检测两个椭圆是否碰撞
//bool checkEllipseCollision(const Ellipse& ellipse1, const Ellipse& ellipse2) {
//    float dx = ellipse1.x - ellipse2.x;
//    float dy = ellipse1.y - ellipse2.y;
//    float distance = std::sqrt(dx * dx + dy * dy);
//    return distance <= (ellipse1.width / 2 + ellipse2.width / 2);
//}
//以上代码用于检测两个椭圆是否相交--------------------------------------------------------------------------------------------------------------------