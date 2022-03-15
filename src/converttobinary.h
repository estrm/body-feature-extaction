#ifndef __CONVERT_TO_BINARY_H__
#define __CONVERT_TO_BINARY_H__

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace std;

/*
 * 作用: 处理原图，得到 黑白二值图
 * get_BinaryImage:        对原图像进行处理，得到黑白二值图
 * fillHole:            填充轮廓内部的区域
 * RemoveSmallRegion:    移除黑白二值图中多余的小区域
 */
void get_BinaryImage(const cv::Mat srcImage, cv::Mat &dstBw); // 对原图像进行处理，得到黑白二值图
void fillHole(const cv::Mat srcBw, cv::Mat &dstBw);           // 填充轮廓内部的区域
void RemoveSmallRegion(cv::Mat& Src, cv::Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode); // 移除多余的小区域

#endif // __CONVERT_TO_BINARY_H__
