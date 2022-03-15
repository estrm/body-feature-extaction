#include "hand.h"

/*
 * 研究对象: 人体正面图
 * 函数: void GetHand(cv::Mat& edge, struct hand * my_hand);
 * 方法：通过人体正面图
 *        (从左到右) 至上而下，第一个像素点为 255 的像素点 即为 左手 坐标
 *        (从右到左) 至上而下，第一个像素点为 255 的像素点 即为 右手 坐标
 */

void GetHand(cv::Mat& edge, struct hand * my_hand) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;  //source image 行数
    int colNumber = outImage.cols;  //source image 列数

    struct left_hand left_hand = {0, 0};
    uchar* data_left;

    /*
     * 行循环区间: [ 100, rowNumber - 150]
     * 预设 手的坐标 在[100, rowNumber - 150] 之间
     *
     */
    int row_start = 0; 
    int row_end = 0;
    row_start = 100;
    row_end = rowNumber - 200;

    // 寻找人体正面黑白二值图 左手坐标
    for (int i = 0; i < colNumber ; i++) {           // 列循环
        for (int j = row_start; j < row_end; j++) {  // 行循环
            data_left = outImage.ptr<uchar>(j);      // 获得第j行的首地址
            if (data_left[i] == 255) {
                left_hand.x = i;
                left_hand.y = j;
                break;
            }
        }
        if (left_hand.x > 0) {
            break;
        }
    }

    /******

    BUG 2017年3月21日22:41:26
    
    ****/
    struct right_hand right_hand = { 0, 0 };
    // 寻找人体正面黑白二值图 左手坐标
    uchar* data_right;
    for (int i = colNumber - 1; i > 0; i--) {        // 列循环
        for (int j = row_start; j < row_end; j++) {  // 行循环
            data_right = outImage.ptr<uchar>(j);     // 获得第 j 行的首地址
            if (data_right[i] == 255) {
                right_hand.x = i;
                right_hand.y = j;
                break;
            }
        }
        if (right_hand.x > 0) {
            break;
        }
    }

    /*
     * 获取 左手坐标 和 右手坐标
     */
    my_hand->my_left_hand  = left_hand;
    my_hand->my_right_hand = right_hand;
}
