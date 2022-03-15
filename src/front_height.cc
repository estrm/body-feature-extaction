#include "front_height.h"

/*
 * 研究对象: 人体正面图
 * 方法：通过人体正面
 *       至上而下遍历像素点，第一个 255 的点为 head
 *       至下而上遍历像素点，第一个 255 的点为 foot
 */

void GetHeight(cv::Mat& edge, int * head, int * foot) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;    //source image 行数
    int colNumber = outImage.cols;  //source image 列数
    int count = 0;                    //count: 存储每一行像素值为255的像素的个数

    /*
     * up  : 人体正面黑白二值图上限, 头顶坐标
     * down: 人体正面黑白二值图下限, 脚底坐标
     */
    int up = 0, down = 0;

    uchar* data;
    // 寻找人体正面黑白二值图上限 up 头顶坐标
    for (int i = 0; i < rowNumber; i++) {       // 行循环
        data = outImage.ptr<uchar>(i);          // 获得第i行的首地址
        for (int j = 0; j < colNumber; j++) {   // 列循环
            if (data[j] == 255) {
                up = i;
                break;
            }
        }
        if (up > 0){
            break;
        }
    }

    // 寻找人体正面黑白二值图下限 down 脚底坐标
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

    /*
     * head： 头顶坐标
     * foot:  脚底坐标
     */
    *head = up;
    *foot = down;
}
