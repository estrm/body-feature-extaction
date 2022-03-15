#include "waist.h"

using namespace std;

int FindWaist(cv::Mat& edge, const FindArea WaistArea) {
    cv::Mat outImage = edge.clone();
    int up = 0;                 // 腰部区间上限
    int down_x = 0, down_y = 0; // 腰部区间下限

    up = WaistArea.UpperLimit;
    down_x = WaistArea.LowerLimit.x;
    down_y = WaistArea.LowerLimit.y;

    int count = 0; //count: 存储每一行像素值为255的像素的个数

    vector<int>sum;
    // 双层循环，遍历所有的像素值
    for (int i = up + 10; i < down_y - 3; i++) { // up -> down_y
        uchar* data = outImage.ptr<uchar>(i); // 获得第i行的首地址
        count = 0;

        // 向右搜索
        for (int j = down_x; j < edge.cols; j++) { // 列循环
            if (data[j] == 255) {
                count++;
            }
            if (data[j] == 0) {
                break;
            }
        }

        // 向左搜索
        for (int j = down_x; j > 0; j--) { // 列循环
            if (data[j] == 255) {
                count++;
            }
            if (data[j] == 0) {
                break;
            }
        }

        // 像素数据压入sum
        sum.push_back(count);
    }

    // 结果 默认 MIN (sum) = 腰
    int result = 0, flag = 0;
    result = edge.cols;

    for (int i = 0; i < sum.size(); i++){
        if (sum[i] < result) {
            result = sum[i];
            flag = i;
        };
    }

    cout << endl;
    cout << "Waist        line[" << flag + up + 10 << "]" << endl;
    return ( result );
}
