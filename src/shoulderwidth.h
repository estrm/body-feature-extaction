#ifndef __SHOULD_WIDTH_H__
#define __SHOULD_WIDTH_H__

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace std;

void pixSum(cv::Mat& edge, vector<int>& sum);                        // 计算黑白二值图每一行像素点的个数
void pixSum_side(cv::Mat& edge, vector<int> &sum);                   // 采用左右像素坐标相减的方法求 像素值 = 255 的像素点的个数
void getGradient(vector<int>& sumpx, vector<double> &pointgradient); // 计算像素数组 `sum` 的梯度，结果保存在vector `pointgradient`
int  MaxGradient(vector<double>arr);                                 // 寻找梯度数组 `pointgradient`中的最大值

#endif // __SHOULD_WIDTH_H__
