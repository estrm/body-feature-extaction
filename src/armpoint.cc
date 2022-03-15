#include "armpoint.h"
/*
 * leftpoint1   :  左活动点
 * right        :  右活动点
 * middlepoint1 :  活动中点
 */
struct allarmpit leftpoint1, rightpoint1, middlepoint1;

/*
 * void GetArmpit(); 获取 腋窝点的坐标
 * 参数:
 *        struct hand * my_hand_y_line:  起始搜索行的行序号
 *        struct armpit * my_armpit   :  目标 腋窝点 的坐标
 */
void GetArmpit(cv::Mat& edge, const struct find_armpit_start_line * start_line, struct armpit * my_armpit) {
    cv::Mat outImage = edge.clone();
    
    // 求左胳膊点的坐标
    struct allarmpit leftarm;
    get_left_arm_point (outImage, start_line->left_start_line, &leftarm);

    // 求右胳膊点的坐标
    struct allarmpit rightarm;
    get_right_arm_point(outImage, start_line->right_start_line, &rightarm);

    my_armpit->leftarmpit  = leftarm;
    my_armpit->rightarmpit = rightarm;
}

// 寻找 left armpit 分叉点的坐标
void get_left_arm_point(cv::Mat& edge, int start_line, struct allarmpit *left_arm) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;    //原图片行数
    int colNumber = outImage.cols;  //原图片列数
    // 初始化 leftpoint1, rightpoint1, middlepoint1 点的坐标
    leftpoint1.x = 0;
    leftpoint1.y = 0;

    rightpoint1.x = 0;
    rightpoint1.y = 0;

    middlepoint1.x = (int)(colNumber / 2); // x 为图片横坐标
    middlepoint1.y = start_line - 50;              // y 为图片纵坐标


    // 计算 left arm 种子搜索点
    for (int i = middlepoint1.y; i > 0; i--) {
        uchar *data = outImage.ptr<uchar>(i);
        // 向左搜索,寻找种子点
         for (int j = middlepoint1.x; j > 0; j--) {
            if (data[j] == 0) {
                rightpoint1.x = j;
                rightpoint1.y = i;
                for (; j > 0; j--){
                    if (data[j] == 255){
                        leftpoint1.x = j;
                        leftpoint1.y = i;
                        break;
                    }
                }

                middlepoint1.x = (rightpoint1.x + leftpoint1.x ) / 2;
                middlepoint1.y = i;
                break;
            }
        }
        if (rightpoint1.x != 0 && leftpoint1.x != 0) {
            break;
        }
    }

    // 从 middlepoint1 开始竖直向上搜索，直到搜索到双腿的分叉点
    for (int i = middlepoint1.y; i > 0; i--) {
        uchar* data = outImage.ptr<uchar>(i);

        // data[middlepoint1.x] = 255 时，向左右方向搜索
        if (data[middlepoint1.x] == 255) {
            middlepoint1.y = i;

            // 向左边10个点搜索
            int count = 0;
            for (int j = 1; j <= 10; j++) {
                if (data[middlepoint1.x - j] == 255) {
                    count++;
                }
            }
            if (count > 5) {
                // middlepoint1 = left leg 的边界
                leftpoint1.x = middlepoint1.x;
                leftpoint1.y = middlepoint1.y;

                for (int j = 1; j < colNumber; j++) {
                    if (data[middlepoint1.x + j] == 255) {
                        // 右边的点
                        rightpoint1.x = middlepoint1.x + j;
                        rightpoint1.y = middlepoint1.y;
                        break;
                    }
                }
            }
            else {
                // middlepoint1 为right leg 的边界
                rightpoint1.x = middlepoint1.x;
                rightpoint1.y = middlepoint1.y;

                for (int j = 1; j > 0; j++) {
                    if (data[middlepoint1.x - j] == 255) {
                        leftpoint1.x = middlepoint1.x - j;
                        leftpoint1.y = middlepoint1.y;
                        break;
                    }
                }
            }
            // middlepoint1
            middlepoint1.x = (int)((leftpoint1.x + rightpoint1.x) / 2);
        }

        else {
            ;
        }

        // 找到双腿分叉点的坐标
        if (middlepoint1.x == leftpoint1.x || middlepoint1.x == rightpoint1.x) {
            break;
        }
        else {
            ;
        }
    }

    // 得到 左胳膊的坐标
    left_arm -> x = middlepoint1.x;    // 双腿分叉点的行坐标
    left_arm -> y = middlepoint1.y;    // 双腿分叉点的列坐标 

    //测试输出结果
    //cout << "left_arm x: " << left_arm -> x << endl;
    //cout << "left_arm y: " << left_arm -> y << endl;
}

// 寻找 right armpit 的分叉点
void get_right_arm_point(cv::Mat& edge, int start_line, struct allarmpit *right_arm) {
    cv::Mat outImage = edge.clone();
    int rowNumber = outImage.rows;  //原图片行数
    int colNumber = outImage.cols;  //原图片列数
    // 初始化 leftpoint1, rightpoint1, middlepoint1 点的坐标
    leftpoint1.x = 0;
    leftpoint1.y = 0;

    rightpoint1.x = 0;
    rightpoint1.y = 0;

    middlepoint1.x = (int)(colNumber / 2); // x 为图片横坐标
    middlepoint1.y = start_line - 50;      // y 为图片纵坐标

    /*
     * 种子搜索点的位置如何选取
     * ? 先找到手的坐标
     *
     *
     */
    // 计算 right arm 种子搜索点
    for (int i = middlepoint1.y; i > 0; i--) {
        uchar *data = outImage.ptr<uchar>(i);
        // 向右边搜索,寻找种子点
        for (int j = middlepoint1.x; j < colNumber - 1; j++) {
            if (data[j] == 0) {
                leftpoint1.x = j;
                leftpoint1.y = i;
                for (; j < colNumber - 1; j++){
                    if (data[j] == 255){
                        rightpoint1.x = j;
                        rightpoint1.y = i;
                        break;
                    }
                }

                middlepoint1.x = (rightpoint1.x + leftpoint1.x) / 2;
                middlepoint1.y = i;
                break;
            }
        }
        if (rightpoint1.x != 0 && leftpoint1.x != 0) {
            break;
        }
    }

    // 从 middlepoint1 开始竖直向上搜索，直到搜索到双腿的分叉点
    for (int i = middlepoint1.y; i > 0; i--) {
        uchar* data = outImage.ptr<uchar>(i);

        // data[middlepoint1.x] = 255 时，向左右方向搜索
        if (data[middlepoint1.x] == 255) {
            middlepoint1.y = i;

            // 向左边10个点搜索
            int count = 0;
            for (int j = 1; j <= 10; j++) {
                if (data[middlepoint1.x - j] == 255) {
                    count++;
                }
            }
            if (count > 5) {
                // middlepoint1 = left leg 的边界
                leftpoint1.x = middlepoint1.x;
                leftpoint1.y = middlepoint1.y;

                for (int j = 1; j < colNumber; j++) {
                    if (data[middlepoint1.x + j] == 255) {
                        // 右边的点
                        rightpoint1.x = middlepoint1.x + j;
                        rightpoint1.y = middlepoint1.y;
                        break;
                    }
                }
            }
            else {
                // middlepoint1 为right leg 的边界
                rightpoint1.x = middlepoint1.x;
                rightpoint1.y = middlepoint1.y;

                for (int j = 1; j > 0; j++) {
                    if (data[middlepoint1.x - j] == 255) {
                        leftpoint1.x = middlepoint1.x - j;
                        leftpoint1.y = middlepoint1.y;
                        break;
                    }
                }
            }
            // middlepoint1
            middlepoint1.x = (int)((leftpoint1.x + rightpoint1.x) / 2);
        }

        else {
            ;
        }

        // 找到双腿分叉点的坐标
        if (middlepoint1.x == leftpoint1.x || middlepoint1.x == rightpoint1.x) {
            break;
        }
        else {
            ;
        }
    }

    // 得到 左胳膊的坐标
    right_arm -> x = middlepoint1.x;    // 双腿分叉点的行坐标
    right_arm -> y = middlepoint1.y;    // 双腿分叉点的列坐标

    //测试输出结果
    //cout << "right_arm x: " << left_arm -> x << endl;
    //cout << "right_arm y: " << left_arm -> y << endl;
}
