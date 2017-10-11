#include"imageprocess.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

Mat preprocessImage(Mat &src);

Mat locatePoints(Mat& img);

//对每个小区域，定义一个结构体来存储相关的信息：点集、拟合出的椭圆中心点、区域内亮度值
struct RegionValue{
    vector<Point> points;
    Point center;
    float value;

};

int main()
{
//    VideoCapture cap("E:\\20170914-FindPoints\\111\\111.avi");
//    if(!cap.isOpened())
//    {
//        cout << "Fail to open the video!" << endl;
//        return 1;
//    }

//    while(1)
//    {
//        Mat frame;
//        cap >> frame;
//        if(frame.empty())
//        {
//            cout << "the end of the video" << endl;
//            return 0;
//        }

//        imshow("video", frame);

//        waitKey(33);

//    }

    //读图片
    Mat src = imread("E:\\20170914-FindPoints\\1.jpg");
    if(!src.data)
    {
        cout << "Cannot open the image" << endl;
        return 1;
    }

    //预处理
    ImageProcess image_process;

//    Mat pre_img;
//    pre_img = image_process.preProcessImage(src);
//    imshow("pre", pre_img);

    vector<CaptureInfo> captures;
    captures = image_process.locatePoint(src);//输出所有的捕手

    //计算每个捕手的平均亮度值
    //注意：输入的为灰度图像
    Mat gray_img;
    cvtColor(src, gray_img, CV_BGR2GRAY);

    //将捕手的几何参数保存到csv文件
    ofstream file("./Points.csv");
    file << "Center.x,Center.y,Left.x,Left.y,Right.x,Right.y,Top.x,Top.y,Bottom.x,Bottom.y,avg_value\n";
    if(file.is_open())
    {
        for(int i = 0; i< captures.size(); i++)
        {
            file << captures.at(i).Center.x << "," << captures.at(i).Center.y << ","
                 << captures.at(i).Left.x << "," << captures.at(i).Left.y << ","
                 << captures.at(i).Right.x << "," << captures.at(i).Right.y << ","
                 << captures.at(i).Top.x << "," << captures.at(i).Top.y << ","
                 << captures.at(i).Bottom.x << "," << captures.at(i).Bottom.y << ","
                 << image_process.calcPixelValue(gray_img, captures.at(i)) << "\n";
        }
    }
    file.close();

//    //计算每个捕手的平均亮度值
//    //注意：输入的为灰度图像
//    Mat gray_img;
//    cvtColor(src, gray_img, CV_BGR2GRAY);
//    for(int i = 0; i < captures.size(); i++)
//    {
//        image_process.calcPixelValue(gray_img, captures.at(i));
//    }


    imshow("Original", src);
    waitKey(0);

    return 0;
}


