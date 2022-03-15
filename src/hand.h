#ifndef __HAND_H__
#define __HAND_H__

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace std;

/*
 * 运算对象: 人体正面图
 * 函数作用：计算人体正面图 左手坐标 和 右手坐标
 * 方法：
 *    计算正面图
 *    (横坐标从 0 开始)   至上而下遍历像素点，= 255 第一个像素点的坐标即为 左手 坐标
 *    (横坐标从 最大开始) 至上而下遍历像素点，= 255 第一个像素点的坐标即为 右手 坐标
 */

// left_hand: 结构体 左手坐标
typedef struct left_hand {
    int x;
    int y;
} left_hand;

// right_hand: 结构体 右手坐标
typedef struct right_hand {
    int x;
    int y;
} right_hand;

// hand: 结构体 手的坐标
typedef struct hand {
    left_hand    my_left_hand;
    right_hand  my_right_hand;
} hand;

void GetHand(cv::Mat& edge, struct hand * my_hand);

#endif // __HAND_H__
