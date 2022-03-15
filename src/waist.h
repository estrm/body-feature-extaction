#ifndef __WAIST_H__
#define __WAIST_H__

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

/*
 * 寻找腰部坐标
 *     搜索区间 A [腋窝, 双腿分叉点] 
 *     方法: 遍历 A 区间, 记录每行像素点的总数(不包括手臂)，像素最少的即为 腰部
 *
 */

// position of single point
typedef struct point {
    int x;
    int y;
} point;

// Area which find the waist
typedef struct FindArea {
    int UpperLimit;
    point LowerLimit;
} FindArea;

// function to get waist width: return "waist width"
int FindWaist(cv::Mat& edge, const FindArea WaistArea);

#endif // __WAIST_H__
