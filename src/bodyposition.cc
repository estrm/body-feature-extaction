#include "bodyposition.h"
#include "front_height.h"
#include "middlepoint.h"
#include "armpoint.h"

/*
 * bodypostion.h        将人体结构分区 [ head、neck、armpit、hand、legpoint、foot ]
 * front_height.h        求人体正面图 头顶坐标 和 脚底坐标
 * middlepoint.h        求双腿分叉点的坐标
 * armpoint.h            求胳膊分叉点的坐标
 */

void GetBodyPosition(cv::Mat& edge, struct bodyp * body) {
    cv::Mat Dst = edge.clone();
    int rowNumber = Dst.rows; //source image 行数
    int colNumber = Dst.cols; //source image 列数

    /*
     *  void GetHeight(): 获取头顶和脚底的坐标 -> front_height.h
     *  head_tmp: 头顶坐标
     *  foot_tmp: 脚底坐标
     */
    int head_tmp = 0, foot_tmp = 0;
    GetHeight(Dst, &head_tmp, &foot_tmp);
    body->head = head_tmp;
    body->foot = foot_tmp;

    /*
     * void GetHand(); 获取手的坐标
     * hand_tmp: 结构体 存储手的坐标 ( 左手坐标 + 右手坐标)
     */
    struct hand hand_tmp;
    GetHand(Dst, &hand_tmp);
    body->my_hand = hand_tmp;

    /*
     * void findthepoint(); 获取双腿分叉点的坐标 -> middlepoint.h
     * legpoint_position: 结构体 存储双腿分叉点的坐标
     */
    struct livepoint legpoint_position;
    findthepoint(Dst, &legpoint_position);
    body->legpoint = legpoint_position;

    /*
     * void GetArmpit(cv::Mat& edge, struct hand * my_hand_y_line, struct armpit * my_armpit); -> armpoint.h
     * 获取 armpit 的坐标
     * struct armpit armpit_position:            结构体 存储 armpit 的坐标
     * struct find_armpit_start_line start_line: 结构体 存储 armpit 起始搜索行
     */
    struct armpit armpit_position;
    struct find_armpit_start_line start_line = {0, 0};
    // 左手
    if ( body->my_hand.my_left_hand.y > body->legpoint.y ) {
        start_line.left_start_line = body->my_hand.my_left_hand.y;
    }
    else {
        start_line.left_start_line = body->my_hand.my_left_hand.y;
    }
    // 右手
    if ( body->my_hand.my_right_hand.y > body->legpoint.y ) {
        start_line.right_start_line = body->my_hand.my_right_hand.y;
    }
    else {
        start_line.right_start_line = body->my_hand.my_right_hand.y;
    }

    GetArmpit(Dst, &start_line, &armpit_position);
    body->my_armpit = armpit_position;
    /******************************************************************/
}
