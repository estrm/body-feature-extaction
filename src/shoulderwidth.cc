#include "shoulderwidth.h"

void pixSum(cv::Mat& edge, vector<int>& sum);                        // 计算黑白二值图每一行像素点的个数
void pixSum_side(cv::Mat& edge, vector<int> &sum);                   // 采用左右像素坐标相减的方法求 像素值 = 255 的像素点的个数
void getGradient(vector<int>& sumpx, vector<double> &pointgradient); // 计算像素数组 `sum` 的梯度，结果保存在vector `pointgradient`
int  MaxGradient(vector<double>arr);                                 // 寻找梯度数组 `pointgradient`中的最大值

/*
 * void pixSum
 * funciton: 计算每一行像素值为255的像素点的总个数
 * 参数
 *    edge: 原图像
 *    sum : 存储每一行像素值为 255 的像素点的个数
 */
void pixSum(cv::Mat& edge, vector<int> &sum) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;  //source image 行数
    int colNumber = outImage.cols;  //source image 列数
    int count = 0;                  //count: 存储每一行像素值为255的像素的个数

    // 双层循环，遍历所有的像素值
    for (int i = 0;i < rowNumber;i++) {       // 行循环
        uchar* data = outImage.ptr<uchar>(i); // 获得第i行的首地址
        count = 0;
        for (int j = 0;j < colNumber;j++) {   // 列循环
            if (data[j] == 255) {
                count++;
            }
        }
        sum.push_back(count);
    }
}

/*
 * void pixSum
 * funciton : 每一行像素的值，左边一个为 255 的像素的位置坐标 - 右边第一个像素值为 255的像素点的位置坐标
 * 用于处理以下情况  1. 人体轮廓检测效果较好 2.人体轮廓内部填充效果不佳
 * 参数 : edge: 原图像
 *   sum: 存储每一行像素值为 255 的像素点的个数
 */
void pixSum_side(cv::Mat& edge, vector<int> &sum) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;  //行数
    int colNumber = outImage.cols;  //列数

    int targetPoint_Left = 0;  // 左边第一个像素值等于 255 的像素点的坐标
    int targetPoint_Right = 0; // 右边第一个像素值等于 255 的像素点的坐标

    int count = 0; //count: 存储每一行像素值为255的像素的个数
    // 双层循环，遍历所有的像素值
    for (int i = 0;i < rowNumber;i++) {       // 行循环
        uchar* data = outImage.ptr<uchar>(i); // 获得第i行的首地址
        count = 0;
        targetPoint_Left = 0;
        targetPoint_Right = 0;

        for (int j = 0;j < colNumber;j++) { // 列循环，寻找 targetPoint_Left
            if (data[j] == 255) {
                targetPoint_Left = j;
                break;
            }
        }
        for (int j = colNumber - 1;j >= targetPoint_Left;j--) { // 列循环
            if (data[j] == 255) {
                targetPoint_Right = j;
                break;
            }
        }
        count = (targetPoint_Right - targetPoint_Left + 1);
        sum.push_back(count);
    }
}

/*
 * void getGradient();
 * function: 计算像素矩阵的梯度
 * 参数:
 * vector<int>& sumpx: 每一行像素值为 255 的个数
 * vector<double>& pointgradient: 存储每一行像素的梯度
 */
void getGradient(vector<int>& sumpx, vector<double> &pointgradient) {
    size_t length = sumpx.size();
    double count = 0; // 临时存储每一行的梯度
    if (length == 0) {
        cout << "像素数据错误！" << endl;
    }
    else if (length == 1) {
        count = sumpx[0];
        pointgradient.push_back(count);
    }
    else if (length == 2) {
        count = sumpx[1] - sumpx[0];
        pointgradient.push_back(count);
        pointgradient.push_back(count);
    }
    else {
        // pointgradient[0]
        count = (sumpx[1] - sumpx[0]);
        pointgradient.push_back(count);

        // pointgradient[1] ... pointgradient[length -2]
        for (int i = 1; i < int(length - 1);i++) {
            count = (sumpx[i + 1] - sumpx[i - 1]) * 1.0 / 2;
            pointgradient.push_back(count);
        }

        // pointgradient[length - 1]
        count = (sumpx[length - 1] - sumpx[length - 2]);
        pointgradient.push_back(count);
    }
}

/*
 * int MaxGradient();
 * function:  寻找图片前半部分，像素梯度的最大值的编号，从而找到 肩膀所在的行数
 * 参数    :
 *       vector<float>arr: 梯度数据
 *
 * 待改进:
 * 去除干扰项，比如: 头顶 可能是梯度最大值的点
 */
int MaxGradient(vector<double>arr) {
    size_t arr_length = 0;
    double MaxGrad_value = 0; // 梯度最大值的值 
    int MaxGrad_number   = 0; // 梯度最大值的编号

    arr_length = arr.size();
    // 肩膀区域，一般在图片的上半部分，所以理论上只需寻找 arr 数组前半部分的最大值
    // arr_length /= 2;
    //printf("arr_length : %d\n", arr_length);

    for (int i = 0;i < int(arr_length);i++) {
        if (arr[i] > 0) {
            //for (i = i + 200; i < int(arr_length); i++) {
            for (;i < int(arr_length);i++) {
                if (arr[i] > MaxGrad_value) {
                    MaxGrad_value = arr[i];
                    MaxGrad_number = i;
                }
            }
        }
    }
    return MaxGrad_number;
}
