/*
 * Arthor: Sun Yiming
 * Date:   May 8th, 2017
 * Email:  zscandyz@gmail.com
 *
 * Description: The Project is about Computer graphics.
 * It can self-collect the key data through a front and side image.
 * BodyEdgeDetection is only used in Eduction.
 *
 * Copyright (c) 2017- 2019 Sun Yiming <zscandyz[at]gmail[dot]com>
 * All rights reserved.
 */

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>
#include <fstream>

/*
 * converttobinary.h    转原图为黑白二值图
 * shoulderwidth.h      求肩宽
 * side_height.h        从侧面图，求人体身高
 * armpoint.h           求胳膊分叉点的坐标 [ 左胳膊 + 右胳膊 ]
 * bodypostion.h        将人体结构分区 [ head、neck、armpit、hand、legpoint、foot ]
 * waist.h              测量人体腰围
 */

#include "converttobinary.h"
#include "shoulderwidth.h"
#include "side_height.h"
#include "bodyposition.h"
#include "point_gradient.h"
#include "waist.h"

using namespace std;

static cv::Mat show(cv::Mat edge, int x, int y);
static cv::Mat draw_position(cv::Mat edge, int line);

// __entry function
int main()
{
    /*
     * while循环: 输入原图片，进行测试，检测测试效果
     *        while (char(waitKey(1)) != 'q') { } // 按 'q' 键退出while循环
     * 按 'q' 键退出while循环; 关闭窗口, 即可输入下一张图片
     */
    int outflag = 0;
    while (1) {
        outflag++;
        cv::Mat srcImage;
        string ImageName = "/0";
        cout << "input image path:(ENTER 'q' to exit) " << endl;

        cin >> ImageName;
        // 按 'q' 或 'Q' 退出程序
        if (ImageName == "q" || ImageName == "Q") {
            return 0;
        }

        // 读取图片 ImageName
        srcImage = cv::imread(ImageName);

        /*
         * while(!srcImage.data){ }: 获取正确的图片文件路径
         */
         while (!srcImage.data) {
            cout << "error to read image." << endl;
            cout << "input again: ";
            cin >> ImageName;
            if (ImageName == "q" || ImageName == "Q") {
                return 0;
            }
            srcImage = cv::imread(ImageName);
            cout << endl;
        }

        /*
         * Dst : 保存黑白二值图
         * get_BinaryImage(): 处理原图片为黑白二值图;
         */
        cv::Mat Dst;
        get_BinaryImage(srcImage, Dst);

        /***********  通过侧面图计算人体像素值和实际长度的比例  **********/
        double height = 170.00 * 10; // 假设实际身高为 170.00 * 10 mm
        double ratio = 0.0;             // ratio 像素身高比例
        GetRatio(Dst, height, &ratio);
        cout << "one pixel = " << ratio << " mm" << endl;
        /*****************************************************************/

        cv::Mat DSTSHOW = Dst.clone();

        // 声明: 人体区段结构体
        struct bodyp pp;
        GetBodyPosition(Dst, &pp);
        cout << endl;
        cout << "|--------  FRONT ---------|" << endl;
        cout << "head         : " << pp.head << endl;
        cout << "foot         : " << pp.foot << endl;
        cout << "legpoint     : " << pp.legpoint.x << " , " << pp.legpoint.y << endl;
        cout << "left  armpit : " << pp.my_armpit.leftarmpit.x  << " , " << pp.my_armpit.leftarmpit.y << endl;
        cout << "right armpit : " << pp.my_armpit.rightarmpit.x << " , " << pp.my_armpit.rightarmpit.y << endl;
        cout << "left  hand   : " << pp.my_hand.my_left_hand.x  << " , " << pp.my_hand.my_left_hand.y << endl;
        cout << "right hand   : " << pp.my_hand.my_right_hand.x << " , " << pp.my_hand.my_right_hand.y << endl;

        /***************** 求腰部 ****************************************/
        struct FindArea WaistArea;
        WaistArea.UpperLimit = (pp.my_armpit.leftarmpit.y > pp.my_armpit.rightarmpit.y) ? (pp.my_armpit.rightarmpit.y) : (pp.my_armpit.leftarmpit.y);
        WaistArea.LowerLimit.x = pp.legpoint.x;
        WaistArea.LowerLimit.y = pp.legpoint.y;

        int WaistWidth = 0;
        WaistWidth = FindWaist(Dst, WaistArea);
        cout << "Waist Width  : " << WaistWidth <<  endl;
        /*****************************************************************/

        /*
         * 划分区域
         */
        // head
        draw_position(DSTSHOW, pp.head);
        // left armpit
        draw_position(DSTSHOW, pp.my_armpit.leftarmpit.y);
        // left hand
        draw_position(DSTSHOW, pp.my_hand.my_left_hand.y);
        // 双腿分叉点
        draw_position(DSTSHOW, pp.legpoint.y);
        // foot
        draw_position(DSTSHOW, pp.foot);

        /**************  斜率特征点  *************************************/
        vector <DstPoint> Left_Gradient_Collection;
        Get_Left_PointGradient(Dst, Left_Gradient_Collection);

        int length_Left_Gradient_Collection = Left_Gradient_Collection.size();

        for (int i = 0; i < length_Left_Gradient_Collection; i++) {
            show(DSTSHOW, Left_Gradient_Collection[i].x, Left_Gradient_Collection[i].y);
        }

        cout << endl;
        cout << "Left gradient feature point num = " << length_Left_Gradient_Collection << endl;

        Left_Gradient_Collection.clear();

        vector <DstPoint> Right_Gradient_Collection;
        Get_Right_PointGradient(Dst, Right_Gradient_Collection);

        int length_Right_Gradient_Collection = Right_Gradient_Collection.size();

        for (int i = 0; i < length_Right_Gradient_Collection; i++) {
            show(DSTSHOW, Right_Gradient_Collection[i].x, Right_Gradient_Collection[i].y);
        }
        cout << "Right gradient feature point num = " << length_Right_Gradient_Collection << endl;

        Right_Gradient_Collection.clear();

        /******************************************************************/

        /****************  计算肩宽  **************************************/
        /*
         * vector<int>sum: 保存每一行像素值 = 255 的点的总数
         * pixSum():       计算每一行像素点 = 255 的个数, saved to 'sum'
         *
         */
        vector<int>sum;
        pixSum(Dst, sum);        // 方法1：遍历像素; vector<int>sum 保存每一行目标像素点的总数
        //pixSum_side(Dst, sum); // 方法2: body两端开始逼近，求左右端点坐标的差值

        /*
         * vector <int> pointgradient: 保存每一行像素数据的梯度
         * getGradient():              计算 sum 数据集的数据梯度
         */
        vector<double> pointgradient;
        getGradient(sum, pointgradient);

        /*
         * 保存人体梯度数据到文件 sum.txt，使用cv::Matlab程序分析梯度数据
         */
        /*
        ofstream ocout;
        ocout.open("C:\\Users\\Administrator\\Desktop\\sum.txt");
        for (int i = 0; i < (int)pointgradient.size(); i++) {
            ocout << pointgradient[i] << endl;
        }
        ocout.close();
        */

        /*
         * 获取肩膀的位置
         * flag: 存储梯度最大值的行数
         */
        int flag = 0;
        flag = MaxGradient(pointgradient);

        // 输出计算结果
        cout << endl;
        cout << "Shoulder     line[" << flag << "]" << endl;
        cout << "Shoulder Width    : " <<  sum[flag] << " pixels." << endl;
        //cout << "换算为厘米  : " << sum[flag] * 0.0606577 << " cm" << endl;

        // show the source image and the binary image.
        cv::Mat temp1;

        // 获得第i行的首地址
        uchar* data = DSTSHOW.ptr<uchar>(flag);

        // 列循环, 将肩膀的位置用特殊颜色标注出来
        draw_position(DSTSHOW, flag);

        /*
         * 以下代码段作用: 直观显示找到的特征点
         */
        cv::destroyAllWindows();

        imshow("DST", DSTSHOW);
        /*
        int pwidth  = GetSystemMetrics(SM_CXSCREEN); // 显示屏宽度
        int pheight = GetSystemMetrics(SM_CYSCREEN); // 显示屏高度

        // the height of source image is lower than height of the Srceen
        if (srcImage.rows < pheight)
        {
            imshow(ImageName, DSTSHOW);
        }
        // height of the source image is higher than height of the Screen, 缩小显示 source image
        else {
            int zoom = (srcImage.rows / pheight) + 1;
            resize(DSTSHOW, temp1, cv::Size(int(srcImage.cols / zoom), int(srcImage.rows / zoom)), (0, 0), (0, 0), 3);
            imshow(ImageName, temp1);
        }
        */

        /*
         * clear data variable, release all resource
         */
        sum.clear();
        pointgradient.clear();
        cout << "--------------------------" << endl;
        cout << endl;
        /*********************************************************************/

        /* save output result
        //imwrite("../../DST.jpg", DSTSHOW);
        char* outpath = (char*)malloc(100*sizeof(char));
        memset(outpath, 0, 100);

        sprintf(outpath, "out_%d.jpg", outflag);
        cout << outpath << endl;
        imwrite("outpath", DSTSHOW);
        //cout << "output image is saved at " << outpath << endl;

        // freepath
        free(outpath);
        */
        cv::waitKey(0);
    }
    return 0;
}

static cv::Mat show(cv::Mat edge, int x, int y) {
    int move = 5;
    // 获取 第y行 的首地址
    uchar *data1 = edge.ptr<uchar>(y);

    // 改变坐标 { [x - move, x + move], y } 像素值的大小
    for (int i = x - move; i < x + move; i++) {
        data1[i] = 180;
    }

    // 改变 { x, [y - move, y + move ] } 像素值的大小
    for (int i = y - move; i < y + move; i++) {

        // 获取 第i行的 首地址
        uchar *data2 = edge.ptr<uchar>(i); //
        data2[x] = 180; //
    }
    return (edge);
}

static cv::Mat draw_position(cv::Mat edge, int line) {
    uchar* data_area = edge.ptr<uchar>(line);
    for (int i = 0; i < edge.cols; i++) {
        data_area[i] = 133;
    }
    return (edge);
}
