#include "middlepoint.h"

using namespace std;
//livepoint: 结构体, 搜索点的坐标
struct livepoint leftpoint, rightpoint, middlepoint;

/*
 * void findthepoint(){} 寻找两腿分叉点的坐标
 * point_x: 行  point_y:列
 *
 */
void findthepoint(cv::Mat& edge, struct livepoint * dstpoint);

void findthepoint(cv::Mat& edge, struct livepoint * dstpoint) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;    //原图片行数
    int colNumber = outImage.cols;    //原图片列数
    //cout << "hang:" << rowNumber << " " << colNumber << endl;
    // 初始化 leftpoint, rightpoint, middlepoint 点的坐标
    leftpoint.x = 0;
    leftpoint.y = 0;

    rightpoint.x = 0;
    rightpoint.y = 0;

    middlepoint.x = (int) (colNumber / 2); // x 为图片横坐标
    middlepoint.y = rowNumber - 1;         // y 为图片纵坐标

    //cout << middlepoint.x << " " << middlepoint.y << endl;

    // 从 (middlepoint.x, middlepoint.y) 开始竖直向上搜索，直到搜索到双腿的分叉点
    for (int i = rowNumber - 1;i > 0;i--) {
        // 获取 第i行 的首地址
        uchar* data = outImage.ptr<uchar>(i);
        
        // data[middlepoint.x] = 255 时，向左右方向搜索
        if (data[middlepoint.x] == 255) {
            middlepoint.y = i;

            // 向左边10个点搜索
            int count = 0;
            for (int j = 1;j <= 10;j++) {
                if (data[middlepoint.x - j] == 255) {
                    count++;
                }
            }
            if (count > 5) {
                // middlepoint = left leg 的边界
                leftpoint.x = middlepoint.x;
                leftpoint.y = middlepoint.y;

                for (int j = 1;j < colNumber;j++) {
                    if (data[middlepoint.x + j] == 255) {
                        // 右边的点
                        rightpoint.x = middlepoint.x + j;
                        rightpoint.y = middlepoint.y;
                        break;
                    }
                }
            }
            else {
                // middlepoint 为right leg 的边界
                rightpoint.x = middlepoint.x;
                rightpoint.y = middlepoint.y;

                for (int j = 1;j > 0;j++) {
                    if (data[middlepoint.x - j] == 255) {
                        leftpoint.x = middlepoint.x - j;
                        leftpoint.y = middlepoint.y;
                        break;
                    }
                }
            }
            // middlepoint
            middlepoint.x = (int)((leftpoint.x + rightpoint.x) / 2);
        }

        else {
            ;
        }
        
        // 找到双腿分叉点的坐标
        if (middlepoint.x == leftpoint.x || middlepoint.x == rightpoint.x) {
            break;
        }
        else {
            ;
        }
    }
    
    // 得到 (point_x,point_y)
    dstpoint -> x = middlepoint.x;    // 双腿分叉点的行坐标
    dstpoint -> y = middlepoint.y;    // 双腿分叉点的列坐标 

    //测试输出结果
    //cout << "middlepoint.x: " << middlepoint.x << endl;
    //cout << "middlepoint.y: " << middlepoint.y << endl;
}
