#include "converttobinary.h"

using namespace cv;
/*
 * void get_BinaryImage()    获得黑白二值图
 * void fillHole()           填充空隙
 * void RemoveSmallRegion()  移除小区域
 */
void get_BinaryImage(const cv::Mat srcImage, cv::Mat &dstBw);
void fillHole(const cv::Mat srcBw, cv::Mat &dstBw);
void RemoveSmallRegion(cv::Mat& Src, cv::Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode);

/*
 * get_BinaryImage() 对原图像进行处理，得到黑白二值图
 * 参数
 *        srcImage: 原图像
 *        dstBw   : 黑白二值图
 */
void get_BinaryImage(const cv::Mat srcImage, cv::Mat &dstBw) {
    //【1】将图片转为灰度图
    cv::Mat srcImage_Gray;
    cvtColor(srcImage, srcImage_Gray, COLOR_BGR2GRAY);
    //cvtColor(srcImage, srcImage_Gray, 6);

    //【2】高斯滤波 消除高斯噪声
    cv::Mat srcImage_Gray_Gauss;
    srcImage_Gray_Gauss = srcImage_Gray;
    GaussianBlur(srcImage_Gray, srcImage_Gray_Gauss, cv::Size(3, 3), 0, 0);

    //【3】canny算子边缘检测，edge保存图像的边缘
    cv::Mat edge = srcImage_Gray_Gauss;
    blur(edge, srcImage_Gray_Gauss, cv::Size(3, 3));//3x3内核降噪
    //Canny(srcImage_Gray_Gauss, edge, 30, 15, 3); // 获得图像边缘, 调节canny的后三位参数
    Canny(srcImage_Gray_Gauss, edge, 30, 15, 3);   // 获得图像边缘    

    //【4】膨胀操作,填充边缘缝隙
    Mat element = getStructuringElement(MORPH_RECT, cv::Size(3, 3));
    //cv::Mat element = getStructuringElement(0, cv::Size(3, 3));

    cv::Mat edge_temp;
    edge_temp = edge;
    for (int i = 0;i < 1;i++) { // 膨胀一次
        dilate(edge_temp, edge, element);
        edge_temp = edge;
    }

    //【5】填充
    for (int i = 0;i < 1;i++) // 填充1次
    {
        fillHole(edge_temp, edge);
        edge_temp = edge;
    }
    threshold(edge_temp, edge, 150, 255, cv::THRESH_BINARY); // 图像二值化
                                                            //blur(edge, edge, Size(3, 3));    //3x3内核降噪

    cv::Mat Dst = cv::Mat::zeros(edge.size(), CV_8UC1);
    RemoveSmallRegion(edge, Dst, 2000, 1, 1);  // 8邻域, 消除 面积 < 2000 个像素点的小区域
                                               // blur(Dst, Dst, Size(3, 3));    //3x3内核降噪
    dstBw = Dst;
}

/*
 * fillHole
 * function: 填充轮廓区域内的部分
 * 参数      srcBw : source image
 *           dstBW :  dst image
 */
void fillHole(const cv::Mat srcBw, cv::Mat &dstBw)
{
    cv::Size m_Size = srcBw.size();

    //延展图像
    cv::Mat Temp = cv::Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());
    srcBw.copyTo(Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)));

    // floofFill 漫水算法
    floodFill(Temp, cv::Point(0, 0), cv::Scalar(255));

    //裁剪延展的图像
    cv::Mat cutImg;
    Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)).copyTo(cutImg);

    dstBw = srcBw | (~cutImg);
}

/*
 * RemoveSmallRegion
 * function: 消除图像中小区域，消除干扰项
 * 参数
 *        Src           : 原图像
 *        Dst           : 目标输出图像
 *        AreaLimit  : 预去除小区域面积
 *        CheckMode  : 0代表去除黑区域, 1代表去除白区域(pix = 255);
 *        NeihborMode: 0代表 4 邻域   , 1代表 8 邻域;
 */
void RemoveSmallRegion(cv::Mat& Src, cv::Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode)
{
    //记录除去的小区域的个数 
    int RemoveCount = 0; 

    /*
     * Pointlabel: 记录每个像素点检验状态的标签，初始化为 0
     *        0 代表 未检查
     *        1 代表 正在检查
     *        2 代表 检查不合格（需要反转颜色
     *        3 代表 检查合格或不需检查
     */
    cv::Mat Pointlabel = cv::Mat::zeros(Src.size(), CV_8UC1);

    //CheckMode = 1, 去除 white 区域 （像素值 = 255）
    if (CheckMode == 1)
    {
        //cout << "Mode: 去除小区域. " << endl;
        for (int i = 0; i < Src.rows; ++i)
        {
            uchar* iData = Src.ptr<uchar>(i); // 获得 cv::Mat::Src 第 i 行首地址
            uchar* iLabel = Pointlabel.ptr<uchar>(i);

            for (int j = 0; j < Src.cols; ++j)
            {
                if (iData[j] < 10)
                {
                    iLabel[j] = 3; // 像素值 < 10, 像素点颜色为黑色，不需要反转
                }
            }
        }
    }
    // 去除black区域 （像素值 = 0）
    else
    {
        //cout << "Mode: 去除孔洞. " << endl;
        for (int i = 0; i < Src.rows; ++i)
        {
            uchar* iData = Src.ptr<uchar>(i); // 取得 Src 第 i 行首地址
            uchar* iLabel = Pointlabel.ptr<uchar>(i);
            for (int j = 0; j < Src.cols; ++j)
            {
                if (iData[j] > 10)
                {
                    iLabel[j] = 3; // 像素值 > 10, 像素点颜色为白色，不需要处理
                }
            }
        }
    }

    // NeihborPos 记录邻域点位置 
    vector <cv::Point2i> NeihborPos;
    NeihborPos.push_back(cv::Point2i(-1,  0));
    NeihborPos.push_back(cv::Point2i( 1,  0));
    NeihborPos.push_back(cv::Point2i( 0, -1));
    NeihborPos.push_back(cv::Point2i( 0,  1));

    if (NeihborMode == 1)
    {
        //cout << "Neighbor mode: 8邻域." << endl;
        NeihborPos.push_back(cv::Point2i(-1, -1));
        NeihborPos.push_back(cv::Point2i(-1,  1));
        NeihborPos.push_back(cv::Point2i( 1, -1));
        NeihborPos.push_back(cv::Point2i( 1,  1));
    }
    else {
        //cout << "Neighbor mode: 4邻域." << endl;
    }

    // NeihborCount = 4（4邻域） NeihborCount = 8（8邻域）
    int NeihborCount = 4 + 4 * NeihborMode; 
    int CurrX = 0, CurrY = 0;

    // 开始检测
    for (int i = 0; i < Src.rows; ++i)
    {
        uchar* iLabel = Pointlabel.ptr<uchar>(i); // 取得 Pointlabel 第 i 行的首地址
        for (int j = 0; j < Src.cols; ++j)
        {
            if (iLabel[j] == 0)
            {
                //********开始 point(i, j)点处的检查***********//
                vector<cv::Point2i> GrowBuffer;          //堆栈，用于存储生长点
                GrowBuffer.push_back(cv::Point2i(j, i));
                Pointlabel.at<uchar>(i, j) = 1;
                int CheckResult = 0;                 //用于判断结果（是否超出大小），0为未超出，1为超出

                for (int z = 0; z < int(GrowBuffer.size()); z++)
                {
                    for (int q = 0; q < NeihborCount; q++) //检查四个邻域点
                    {
                        CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
                        CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
                        if (CurrX >= 0 && CurrX < Src.cols && CurrY >= 0 && CurrY < Src.rows) //防止越界
                        {
                            if (Pointlabel.at<uchar>(CurrY, CurrX) == 0)
                            {
                                GrowBuffer.push_back(cv::Point2i(CurrX, CurrY)); //邻域点加入buffer
                                Pointlabel.at<uchar>(CurrY, CurrX) = 1;      /*更新邻域点的检查标签，避免重复检查
                                                                             1 代表正在检查 */
                            }
                        }
                    }
                }
                if (int(GrowBuffer.size()) > AreaLimit) {
                    CheckResult = 2; //判断结果（是否超出限定的面积area），1为未超出，2为超出
                }
                else {
                    CheckResult = 1; // GrowBuffer.size() < AreaLimit, 待消除的项
                    RemoveCount++;   // 移除小区域的计数 + 1
                }
                for (int z = 0; z < int(GrowBuffer.size()); z++)  //更新Label记录
                {
                    CurrX = GrowBuffer.at(z).x;
                    CurrY = GrowBuffer.at(z).y;
                    Pointlabel.at<uchar>(CurrY, CurrX) += CheckResult;
                }
                //********结束该点处的检查**********//
            }
        }
    }

    CheckMode = 255 * (1 - CheckMode);
    //开始反转面积过小的区域
    for (int i = 0; i < Src.rows; ++i)
    {
        uchar* iData = Src.ptr<uchar>(i);    // 获得 Src 第 i 行的首地址
        uchar* iDstData = Dst.ptr<uchar>(i); // 获得 Dst 第 i 行的首地址
        uchar* iLabel = Pointlabel.ptr<uchar>(i);
        for (int j = 0; j < Src.cols; ++j)
        {
            if (iLabel[j] == 2) // iLabel = 2, 需要反转颜色
            {
                iDstData[j] = CheckMode;
            }
            else if (iLabel[j] == 3) // iLabel = 3, 不需要反转颜色
            {
                iDstData[j] = iData[j];
            }
        }
    }
    //cout << "AreaLimit : " << AreaLimit << endl;
    //cout << RemoveCount << " objects was removed." << endl;
}
