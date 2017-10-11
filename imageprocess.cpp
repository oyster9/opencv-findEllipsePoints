#include "imageprocess.h"

ImageProcess::ImageProcess()
{

}

ImageProcess::~ImageProcess()
{

}

//ͼ��Ԥ����ģ��
//��Ҫ�������ҶȻ����˲�����ֵ��
Mat ImageProcess::preProcessImage(Mat &src)
{
    Mat img_gray;

    cvtColor(src,img_gray,CV_BGR2GRAY);
    GaussianBlur(img_gray, img_gray, Size(5,5),0,0);
//    medianBlur(img_gray, img_gray, 3);

    //ֱ��ͼ���⻯
//        Mat equalize_img;
//        equalizeHist(img_gray, equalize_img);
//        imshow("equalizeHist", equalize_img);

    //ȫ��������ƽ��ֵ��
    Scalar global_C = mean(img_gray);
    //    Ȼ���ֵ��
    Mat global_threshold;
    threshold(img_gray,global_threshold,global_C[0]*1.5, 255, CV_THRESH_BINARY);


    //��Ϊ�м�����̫�������Ƿ������ֵ��
    //�м������ֵ��
    Mat center_region;
    Mat tmp = img_gray.clone();

    center_region = tmp(Rect(img_gray.cols/4,img_gray.rows/4, img_gray.cols*2/4, img_gray.rows*2/4));
    threshold(center_region,center_region,global_C[0]*1.46,255,CV_THRESH_BINARY);

    //���������ֵ��
    Mat edges_region;
    edges_region = img_gray.clone();

    Mat mask = edges_region(Rect(img_gray.cols/4,img_gray.rows/4, img_gray.cols*2/4, img_gray.rows*2/4));
    mask = Mat::zeros(mask.size(),mask.type());
    threshold(edges_region,edges_region,global_C[0]*1.28,255,CV_THRESH_BINARY);

    //�ϲ�(�м������ֵͼ���Ƹ�mask,mask��ͼ��edges_region�ǰ󶨵ģ�
    //��mask�Ĳ������Ƕ�edges_region�ж�ӦROI�Ĳ�����
    center_region.copyTo(mask);

    //    imshow("center_region", center_region);
    //    imshow("edges_region", edges_region);
    //    imshow("global_threshold", global_threshold);

    //��������ֵͼ����̬ѧ����
//    Mat result = Mat::zeros(img_gray.size(), img_gray.type());
//    Mat element = getStructuringElement(MORPH_RECT, Size(2,2));
//    erode(edges_region, result, element);
//        imshow("result", result);

//    return result;

    return edges_region;
}


//��λ����λ�úͻ�ȡ���ַ������㼸�β����ĺ�������״����Բ��ϣ�
//�������õ������x�����ꡢ���ұ�x�����ꡢ
//     �õ����ϱ�y�����ꡢ���±�y������
//     �õ�����x���ꡢ�õ�����y����
//���ص�CaptureInfo���ͽṹ��洢������Ϣ
vector<CaptureInfo> ImageProcess::locatePoint(Mat &img)
{
    Mat draw_image = img.clone();

    Mat img2 = preProcessImage(img);

    //1.������
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //��������
    //ע�⣺findContours()����ȡͼ������ʱ���޸�ԭ����ͼ�������
    //��img2�����ݻᷢ���仯
    findContours(img2,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);

    //2.��ÿ������������
    for(int i = 0; i < contours.size(); i++)
    {
        //������Բ��ʹ��fitEllipse��϶�ά�㼯
        //RotatedRect���ʾƽ���ϵ���ת���Σ����������ε����ĵ㡢size�����ߣ�����ת�Ƕ�
        if(contours.at(i).size()>5)//�����ϵĵ����5������Ϊ��ϵ��㷨��������Ҫ6����
        {
            RotatedRect box = fitEllipse(contours.at(i));

            box.angle = 90; //���Ƕȵ���Ϊȫ��ˮƽ����

            //���ƾ��ο�
            Point2f vertex[4];
            box.points(vertex);
            for(int m =0; m < 4; m++)
            {
//                line(draw_image, vertex[m], vertex[(m+1)%4], Scalar(255,0,0),1);
                //�������ο��4�������ĵ�
//                Point2f tmp;
//                tmp.x = (vertex[m].x+vertex[(m+1)%4].x) / 2.0;
//                tmp.y = (vertex[m].y+vertex[(m+1)%4].y) / 2.0;
//                circle(draw_image,tmp, 1, Scalar(0,0,255), -1);
            }
            circle(draw_image,box.center, 0.5, Scalar(200,200,255), -1);

            //��������ɸѡ���ƣ�����Բ�����С�����˵�
            if( MAX(box.size.width, box.size.height)<5 || MIN(box.size.width, box.size.height)>20 )
            {
                 continue;
            }


            //��һ�ֻ���Բ�ĺ���
            ellipse(draw_image,box,Scalar(0,255,0),1,8);
//            //�ڶ��ֻ���Բ�ĺ���
//            Size axes;//������
//            axes.width = box.size.width/2.0;
//            axes.height = box.size.height/2.0 ;
//            ellipse(draw_image,box.center,axes,90,0,360,Scalar(0,255,0));

            //�����Բ��4������,���ĵ�
            Point2f Left, Right, Top, Bottom;

            Left.x = (vertex[0].x + vertex[3].x) / 2.0; Left.y = (vertex[0].y + vertex[3].y) / 2.0;
            Right.x = (vertex[1].x + vertex[2].x) / 2.0; Right.y = (vertex[1].y + vertex[2].y) / 2.0;
            Top.x = (vertex[0].x + vertex[1].x) / 2.0; Top.y = (vertex[0].y + vertex[1].y) / 2.0;
            Bottom.x = (vertex[2].x + vertex[3].x) / 2.0; Bottom.y = (vertex[2].y + vertex[3].y) / 2.0;



            circle(draw_image,Left, 1.5, Scalar(0,0,255), -1);
            circle(draw_image,Right, 1.5, Scalar(0,0,255), -1);
            circle(draw_image,Top, 1.5, Scalar(0,255,255), -1);
            circle(draw_image,Bottom,1.5, Scalar(0,255,255), -1);

            //������Բ���ĸ����㡢���ĵ�
            _stru_capture.Left = Left;
            _stru_capture.Right = Right;
            _stru_capture.Top = Top;
            _stru_capture.Bottom = Bottom;
            _stru_capture.Center = box.center;

            //������λ�õļ��β�����������
            _vec_captures.push_back(_stru_capture);

//========================================================================================================
            //��֤��Բ�ڵĵ��жϷ����Ƿ���ȷ
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

    //��ʾͼ��
    imshow("captures", draw_image);

    return _vec_captures;

}

//������Բ��Χ�����е�����ֵ�ľ�ֵ
//����Ĳ���Ϊ:1)�Ҷ�ͼ��2)һ����Բ���ĸ����㣬һ�����ĵ�
float ImageProcess::calcPixelValue(Mat &img, CaptureInfo &ell)
{
    //�ж����ص��Ƿ�����Բ��
    //����
    Point2f f1,f2;
    f1.x = (ell.Center.x + ell.Left.x) / 2.0;
    f1.y = (ell.Center.y + ell.Left.y) / 2.0;
    f2.x = (ell.Center.x + ell.Right.x) / 2.0;
    f2.y = (ell.Center.y + ell.Right.y) / 2.0;

    //����
    float A = ell.Center.x - ell.Left.x;

    float avg_value = 0.0, cnt = 0.0;

    //��PF1+PF2 <= A*2,�϶���P����Բ�ڣ�������Ե�㣩
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

    //����ƽ������ֵ
    avg_value = avg_value / cnt;
    cout << "cnt:" << cnt << endl;

    return avg_value;
}
