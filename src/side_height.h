#ifndef __SIDE_HEIGHT_H__
#define __SIDE_HEIGHT_H__

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace std;

/*
 * 运算对象: 人体侧面图
 * 函数作用：计算一个像素对应的尺寸
 * 方法：
 *        1. 计算侧面图，身高像素点的个数
 *        2. 根据人体的实际身高，进一步计算出一个像素对应实际长度的大小
 */
void GetRatio(cv::Mat& edge, double height, double* Pix_Ratio);

#endif // __SIDE_HEIGHT_H__
