#ifndef __ARM_POINT_H__
#define __ARM_POINT_H__

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace std;
/*
 * 运算对象：人体正视图
 * 函数作用: 计算胳膊分叉点的坐标
 * 计算方法：类似于计算双腿分叉点的坐标
 */

// allarmpit 腋窝的坐标
typedef struct allarmpit {
    int x;
    int y;
} allarmpit;

// armpit 腋窝的坐标
typedef struct armpit {
    struct allarmpit leftarmpit;
    struct allarmpit rightarmpit;
} armpit;

// armpit 起始搜索点
typedef struct find_armpit_start_line {
    int left_start_line;
    int right_start_line;
} find_armpit_start_line;

void GetArmpit(cv::Mat& edge, const struct find_armpit_start_line * start_line, struct armpit * my_armpit);
void get_left_arm_point (cv::Mat& edge, int start_line, struct allarmpit * left_arm);
void get_right_arm_point(cv::Mat& edge, int start_line, struct allarmpit * right_arm);

#endif // __ARM_POINT_H__
