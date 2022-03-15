#ifndef __FRONT_HEIGHT_H_
#define __FRONT_HEIGHT_H_

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace std;

/*
 * 运算对象: 人体正面图
 * 函数作用：计算人体正面图 头顶的坐标 和 脚底的坐标
 * 方法：
 *    计算正面图
 *    至上而下遍历像素点，第一个 255 的像素点定义为 head
 *    至下而上遍历像素点，第一个 255 的像素点定义为 foot
 */
void GetHeight(cv::Mat& edge, int * head, int * foot);

#endif // __FRONT_HEIGHT_H_
