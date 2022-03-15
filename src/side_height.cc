#include "side_height.h"

/*
 * 研究对象: 人体侧面图
 * 方法：通过人体侧面图，人体身高的像素点，计算一个像素对应实际像素的尺寸
 */

void GetRatio(cv::Mat& edge, double height,double* Pix_Ratio) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;  //source image 行数
    int colNumber = outImage.cols;  //source image 列数
    int count = 0;                  //count: 存储每一行像素值为255的像素的个数

    /*
     * up  : 人体侧面黑白二值图上限, 头顶坐标
     * down: 人体侧面黑白二值图下限, 脚底坐标
     */
    int up = 0, down = 0;

    uchar* data;
    // 寻找人体侧面黑白二值图上限 up 头顶坐标
    for (int i = 0; i < rowNumber; i++) { // 行循环
        data = outImage.ptr<uchar>(i); // 获得第i行的首地址
        for (int j = 0; j < colNumber; j++) { // 列循环
            if (data[j] == 255) {
                up = i;
                break;
            }
        }
        if (up > 0){
            break;
        }
    }

    // 寻找人体侧面黑白二值图下限 down 脚底坐标
    for (int i = rowNumber - 1; i > 0; i--){
        data = outImage.ptr<uchar>(i);
        for (int j = 0; j < colNumber; j++) {
            if (data[j] == 255) {
                down = i;
                break;
            }
        }
        if (down > 0) {
            break;
        }
    }

    // 计算一个比例 Ratio（一个像素的实际长度）
    *Pix_Ratio = height * 1.0 / (down - up + 1);
    cout << "|------- Side  ---------|" << endl;
    cout << "head position: " << up   << endl;
    cout << "foot position: " << down << endl;
}
