#include "point_gradient.h"

/*
 *  求半侧身体点之间的斜率
 *
 */

/*
 * 判断两点之间的斜率与上一条线段的斜率是否相等
 */

double get_gradient(DstPoint p1, DstPoint p2) {
    // k 斜率
    double k = 0;
    if ( p1.x == p2.x) {
        k = INF;
    }
    else {
        k = (p1.y - p2.y) * 1.0 / (p1.x - p2.x);
    }

    // 返回斜率
    return (k);
}

/*
 * 求左半侧身体边缘 两点之间的斜率，从而求得特征点的集合
 *
 */
void Get_Left_PointGradient(cv::Mat& edge, vector<DstPoint> &Left_Gradient_Collection) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;  //行数
    int colNumber = outImage.cols;  //列数

    /*
     * p1 : 前一个节点
     * p2 : 后一个节点
     */
    DstPoint p1 = { 0, 0 };
    DstPoint p2 = { 0, 0 };

    /*
     * k1 前一线段的斜率
     * k2 后两点之间的斜率
     */
    double k1 = 0.0;
    double k2 = 0.0;


    // 双层循环，遍历所有的像素值
    for (int i = 0; i < rowNumber; i = i + 1) { // 行循环
        uchar* data = outImage.ptr<uchar>(i); // 获得第i行的首地址

        for (int j = 0; j < colNumber; j++) { // 列循环，寻找 左边缘点
            if (data[j] == 255) {
                p2.x = j;
                p2.y = i;

                k2 = get_gradient(p1, p2);
                if (k1 == k2) {
                    Left_Gradient_Collection.pop_back();    // 删除最后一个元素
                    Left_Gradient_Collection.push_back(p2); // 将新元素存入 Left_Gradient_Collection
                    p1 = p2;
                }
                else {
                    Left_Gradient_Collection.push_back(p2);
                    k1 = k2;
                    p1 = p2;
                }
                break;
            }
        }
    }
}

/*
 * 求右半侧身体 两点之间的斜率，从而求得特征点的集合
 *
 */
void Get_Right_PointGradient(cv::Mat& edge, vector<DstPoint> &Right_Gradient_Collection) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;  //行数
    int colNumber = outImage.cols;  //列数

    /*
    * p1 : 前一个节点
    * p2 : 后一个节点
    */
    DstPoint p1 = { 0, 0 };
    DstPoint p2 = { 0, 0 };

    /*
    * k1 前一线段的斜率
    * k2 后两点之间的斜率
    */
    double k1 = 0.0;
    double k2 = 0.0;


    // 双层循环，遍历所有的像素值
    for (int i = 0; i < rowNumber; i = i + 1) { // 行循环
        uchar* data = outImage.ptr<uchar>(i);   // 获得第i行的首地址

        for (int j = colNumber - 1; j >= 0; j--) { // 列循环
            if (data[j] == 255) {
                p2.x = j;
                p2.y = i;
                k2 = get_gradient(p1, p2);
                if (k1 == k2) {
                    Right_Gradient_Collection.pop_back();    // 删除最后一个元素
                    Right_Gradient_Collection.push_back(p2); // 将新元素存入 Left_Gradient_Collection
                    p1 = p2;
                }
                else {
                    Right_Gradient_Collection.push_back(p2);
                    k1 = k2;
                    p1 = p2;
                }

                break;
            }
        }
    }
}
