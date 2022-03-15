#ifndef __BODY_POSITION_H__
#define __BODY_POSITION_H__

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>

#include "middlepoint.h"
#include "hand.h"
#include "armpoint.h"

/*
 * bodyp: 存储body不同区段的坐标
 *
 * 以人体左半边身体为标定，如，左胳膊，左手，左脚
 * (注: 以右半边身体为研究对象，同样可行)
 */
typedef struct bodyp{
    int head;                    // 头顶
    int neck;                    // 脖子
    struct armpit    my_armpit;  // 胳肢窝
    struct hand      my_hand;    // 左手坐标 和 右手坐标
    struct livepoint legpoint;   // 双腿分叉点的坐标
    int foot;                    // 脚底
} bodyp;

/*
 * struct find_armpit_start_line;
 * 结构体，搜索 armpit 的起始行
 */

void GetBodyPosition(cv::Mat& edge, struct bodyp * body);

#endif // __BODY_POSITION_H__
