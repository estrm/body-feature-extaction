#ifndef __POINT_GRADIENT_H_
#define __POINT_GRADIENT_H_

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

#define INF 100000 // 如果两点横坐标相等，斜率无限大，设为 INF

using namespace std;
/*
 * 求半侧身体点之间的斜率，寻找差异点
 *
 */

typedef struct DstPoint {
    int x; // 横坐标
    int y; // 纵坐标
} DstPoint;


void Get_Left_PointGradient (cv::Mat& edge, vector<DstPoint> &Left_Gradient_Collection);
void Get_Right_PointGradient(cv::Mat& edge, vector<DstPoint> &Right_Gradient_Collection);

double get_gradient(DstPoint p1, DstPoint p2);

#endif // __POINT_GRADIENT_H_
