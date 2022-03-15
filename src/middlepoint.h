#ifndef __MIDDLE_POINT_H__
#define __MIDDLE_POINT_H__

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

typedef struct livepoint {
    int x;
    int y;
} livepoint;

/*
 * void findthepoint(): 寻找双腿分叉点的坐标 (point_x, point_y)
 * 方法描述：
 *     step1: 从图片的中轴开始向上遍历
 *     step2: 当遇到 left leg or right leg 的边界时，判断middlepoint是 left leg 还是 right leg，求出left leg and right leg的坐标
 *     step3: middlepoint = (leftpoint + rightpoint) / 2 ;重复 step1, 直到 middlepoint = leftpoint or middlepoint = rightpoint
 */
//void findthepoint(cv::Mat& edge, int *point_x, int *point_y);
void findthepoint(cv::Mat& edge, struct livepoint *dstpoint);

#endif // __MIDDLE_POINT_H__
