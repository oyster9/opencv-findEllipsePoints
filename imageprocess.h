#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H


#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

struct CaptureInfo{
//    float Left_x;
//    float Right_x;
//    float Top_y;
//    float Bottom_y;
//    float Center_x;
//    float Center_y;
    Point2f Left;
    Point2f Right;
    Point2f Top;
    Point2f Bottom;
    Point2f Center;
};

class ImageProcess
{
public:
    ImageProcess();
    ~ImageProcess();

public:
    Mat preProcessImage(Mat &src);
    vector<CaptureInfo> locatePoint(Mat &img);
    float calcPixelValue(Mat &img, CaptureInfo &ell);

private:
    Mat _src;
    CaptureInfo _stru_capture;
    vector<CaptureInfo> _vec_captures;

};

#endif // IMAGEPROCESS_H
