#include "imageprocess.h"

ImageProcess::ImageProcess()
{

}

ImageProcess::~ImageProcess()
{

}

//图像预处理模块
//主要操作：灰度化，滤波，二值化
Mat ImageProcess::preProcessImage(Mat &src)
{
    Mat img_gray;

    cvtColor(src,img_gray,CV_BGR2GRAY);
    GaussianBlur(img_gray, img_gray, Size(5,5),0,0);
//    medianBlur(img_gray, img_gray, 3);

    //直方图均衡化
//        Mat equalize_img;
//        equalizeHist(img_gray, equalize_img);
//        imshow("equalizeHist", equalize_img);

    //全局求亮度平均值，
    Scalar global_C = mean(img_gray);
    //    然后二值化
    Mat global_threshold;
    threshold(img_gray,global_threshold,global_C[0]*1.5, 255, CV_THRESH_BINARY);


    //因为中间区域太亮，考虑分区域二值化
    //中间区域二值化
    Mat center_region;
    Mat tmp = img_gray.clone();

    center_region = tmp(Rect(img_gray.cols/4,img_gray.rows/4, img_gray.cols*2/4, img_gray.rows*2/4));
    threshold(center_region,center_region,global_C[0]*1.46,255,CV_THRESH_BINARY);

    //四周区域二值化
    Mat edges_region;
    edges_region = img_gray.clone();

    Mat mask = edges_region(Rect(img_gray.cols/4,img_gray.rows/4, img_gray.cols*2/4, img_gray.rows*2/4));
    mask = Mat::zeros(mask.size(),mask.type());
    threshold(edges_region,edges_region,global_C[0]*1.28,255,CV_THRESH_BINARY);

    //合并(中间区域二值图像复制给mask,mask与图像edges_region是绑定的，
    //对mask的操作就是对edges_region中对应ROI的操作）
    center_region.copyTo(mask);

    //    imshow("center_region", center_region);
    //    imshow("edges_region", edges_region);
    //    imshow("global_threshold", global_threshold);

    //对整个二值图做形态学处理
//    Mat result = Mat::zeros(img_gray.size(), img_gray.type());
//    Mat element = getStructuringElement(MORPH_RECT, Size(2,2));
//    erode(edges_region, result, element);
//        imshow("result", result);

//    return result;

    return edges_region;
}


//定位捕手位置和获取捕手分子样点几何参数的函数（形状用椭圆拟合）
//包括：该点最左边x轴坐标、最右边x轴坐标、
//     该点最上边y轴坐标、最下边y轴坐标
//     该点中心x坐标、该点中心y坐标
//返回的CaptureInfo类型结构体存储上述信息
vector<CaptureInfo> ImageProcess::locatePoint(Mat &img)
{
    Mat draw_image = img.clone();

    Mat img2 = preProcessImage(img);

    //1.找轮廓
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //查找轮廓
    //注意：findContours()在提取图像轮廓时会修改原输入图像的内容
    //故img2的内容会发生变化
    findContours(img2,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);

    //2.对每个轮廓做处理
    for(int i = 0; i < contours.size(); i++)
    {
        //绘制椭圆，使用fitEllipse拟合二维点集
        //RotatedRect类表示平面上的旋转矩形，包含：矩形的中心点、size（宽、高）、旋转角度
        if(contours.at(i).size()>5)//轮廓上的点大于5个，因为拟合的算法中至少需要6个点
        {
            RotatedRect box = fitEllipse(contours.at(i));

            box.angle = 90; //将角度调整为全部水平放置

            //绘制矩形框
            Point2f vertex[4];
            box.points(vertex);
            for(int m =0; m < 4; m++)
            {
//                line(draw_image, vertex[m], vertex[(m+1)%4], Scalar(255,0,0),1);
                //画出矩形框的4个边中心点
//                Point2f tmp;
//                tmp.x = (vertex[m].x+vertex[(m+1)%4].x) / 2.0;
//                tmp.y = (vertex[m].y+vertex[(m+1)%4].y) / 2.0;
//                circle(draw_image,tmp, 1, Scalar(0,0,255), -1);
            }
            circle(draw_image,box.center, 0.5, Scalar(200,200,255), -1);

            //这里用了筛选机制：若椭圆过大过小均过滤掉
            if( MAX(box.size.width, box.size.height)<5 || MIN(box.size.width, box.size.height)>20 )
            {
                 continue;
            }


            //第一种画椭圆的函数
            ellipse(draw_image,box,Scalar(0,255,0),1,8);
//            //第二种画椭圆的函数
//            Size axes;//长短轴
//            axes.width = box.size.width/2.0;
//            axes.height = box.size.height/2.0 ;
//            ellipse(draw_image,box.center,axes,90,0,360,Scalar(0,255,0));

            //绘出椭圆的4个顶点,中心点
            Point2f Left, Right, Top, Bottom;

            Left.x = (vertex[0].x + vertex[3].x) / 2.0; Left.y = (vertex[0].y + vertex[3].y) / 2.0;
            Right.x = (vertex[1].x + vertex[2].x) / 2.0; Right.y = (vertex[1].y + vertex[2].y) / 2.0;
            Top.x = (vertex[0].x + vertex[1].x) / 2.0; Top.y = (vertex[0].y + vertex[1].y) / 2.0;
            Bottom.x = (vertex[2].x + vertex[3].x) / 2.0; Bottom.y = (vertex[2].y + vertex[3].y) / 2.0;



            circle(draw_image,Left, 1.5, Scalar(0,0,255), -1);
            circle(draw_image,Right, 1.5, Scalar(0,0,255), -1);
            circle(draw_image,Top, 1.5, Scalar(0,255,255), -1);
            circle(draw_image,Bottom,1.5, Scalar(0,255,255), -1);

            //返回椭圆的四个顶点、中心点
            _stru_capture.Left = Left;
            _stru_capture.Right = Right;
            _stru_capture.Top = Top;
            _stru_capture.Bottom = Bottom;
            _stru_capture.Center = box.center;

            //将捕手位置的几何参数存入容器
            _vec_captures.push_back(_stru_capture);

//========================================================================================================
            //验证椭圆内的点判断方法是否正确
//            //addddddddddddddddddddddddddddddddddddddddddddddddddd
//            Point2f f1,f2;
//            f1.x = (box.center.x + Left.x) / 2.0;
//            f1.y = (box.center.y + Left.y) / 2.0;
//            f2.x = (box.center.x + Right.x) / 2.0;
//            f2.y = (box.center.y + Right.y) / 2.0;


//            circle(draw_image,f1,1,Scalar(0,255,0),1);
//            circle(draw_image,f2,1,Scalar(0,255,0),1);

//            float A = box.center.x - Left.x;

//            for(int x = Left.x; x <= Right.x; x++)
//            {
//                for(int y = Top.y; y <= Bottom.y; y++)
//                {
//                    float PF1,PF2;
//                    PF1 = sqrt((x-f1.x)*(x-f1.x) + (y-f1.y)*(y-f1.y));
//                    PF2 = sqrt((x-f2.x)*(x-f2.x) + (y-f2.y)*(y-f2.y));

//                    if( PF1+PF2 <= 2*A)
//                    {
//                       circle(draw_image,Point(x,y),0.5,Scalar(0,0,255));
//                    }
//                }
//            }
//=========================================================================================================
        }
    }

    //显示图像
    imshow("captures", draw_image);

    return _vec_captures;

}

//计算椭圆范围内所有点像素值的均值
//输入的参数为:1)灰度图像，2)一个椭圆的四个顶点，一个中心点
float ImageProcess::calcPixelValue(Mat &img, CaptureInfo &ell)
{
    //判断像素点是否在椭圆内
    //焦点
    Point2f f1,f2;
    f1.x = (ell.Center.x + ell.Left.x) / 2.0;
    f1.y = (ell.Center.y + ell.Left.y) / 2.0;
    f2.x = (ell.Center.x + ell.Right.x) / 2.0;
    f2.y = (ell.Center.y + ell.Right.y) / 2.0;

    //长轴
    float A = ell.Center.x - ell.Left.x;

    float avg_value = 0.0, cnt = 0.0;

    //若PF1+PF2 <= A*2,认定点P在椭圆内（包含边缘点）
    for(int x = ell.Left.x; x <= ell.Right.x; x++)
    {
        for(int y = ell.Top.y; y <= ell.Bottom.y; y++)
        {
            float PF1,PF2;
            PF1 = sqrt((x-f1.x)*(x-f1.x) + (y-f1.y)*(y-f1.y));
            PF2 = sqrt((x-f2.x)*(x-f2.x) + (y-f2.y)*(y-f2.y));

            if( PF1+PF2 <= 2*A)
            {
                avg_value += img.at<uchar>(y,x);
                cnt++;
            }
        }
    }

    //计算平均亮度值
    avg_value = avg_value / cnt;
    cout << "cnt:" << cnt << endl;

    return avg_value;
}
