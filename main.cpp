#include"imageprocess.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

Mat preprocessImage(Mat &src);

Mat locatePoints(Mat& img);

//��ÿ��С���򣬶���һ���ṹ�����洢��ص���Ϣ���㼯����ϳ�����Բ���ĵ㡢����������ֵ
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

    //��ͼƬ
    Mat src = imread("E:\\20170914-FindPoints\\1.jpg");
    if(!src.data)
    {
        cout << "Cannot open the image" << endl;
        return 1;
    }

    //Ԥ����
    ImageProcess image_process;

//    Mat pre_img;
//    pre_img = image_process.preProcessImage(src);
//    imshow("pre", pre_img);

    vector<CaptureInfo> captures;
    captures = image_process.locatePoint(src);//������еĲ���

    //����ÿ�����ֵ�ƽ������ֵ
    //ע�⣺�����Ϊ�Ҷ�ͼ��
    Mat gray_img;
    cvtColor(src, gray_img, CV_BGR2GRAY);

    //�����ֵļ��β������浽csv�ļ�
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

//    //����ÿ�����ֵ�ƽ������ֵ
//    //ע�⣺�����Ϊ�Ҷ�ͼ��
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


