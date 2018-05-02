//
//  Picture.cpp
//  dashboard-identification
//
//  Created by LWS on 2018/4/11.
//  Copyright © 2018年 LWS. All rights reserved.
//

#include "Picture.hpp"

std::vector<Point> getPoints(Mat &image, int value);
void drawLine(cv::Mat &image, double theta, double rho, cv::Scalar color);


Picture::Picture(Mat firstPic) {
    this->firstPic = firstPic;
};

// 显示原图
void Picture::showFirstPic() {
    imshow("原图", this->firstPic);
    waitKey(0);
};

// 获取盘身
void Picture::getPanePic() {
    Mat grayImage; //灰度图
    Mat thre;
    Mat edge; // 降噪图
    //转换为灰度图
    cvtColor(this->firstPic, grayImage, CV_BGR2GRAY);
    //降噪
    blur(grayImage, thre, Size(3,3));
    // 高斯滤波
    GaussianBlur(thre, thre, Size(9, 9), 2, 2);
    // 图像二值化，
    threshold(thre, edge, 90, 255, THRESH_BINARY);
    // 运行Canny算子，3为threshold1，9为threshold2
    Canny(edge, edge, 3, 9);
    this->panePic = edge;
};

// 显示 盘身图片
void Picture::showPanePic() {
    this->getPanePic();
    if(this->panePic.empty()) {
        std::cout << "获取表盘失败" << std::endl;
        return;
    }
    imshow("盘身", this->panePic);
    waitKey(0);
};

// 获取圆心坐标
void Picture::getcenterPoint() {
    
    IplImage *src = cvLoadImage("/Users/admin/Desktop/graduation/pic/meter1.jpeg",0);
    IplImage *dst = cvCreateImage(cvGetSize(src),8,1);
    IplImage *color_dst = cvCreateImage(cvGetSize(src),8,3);
    CvMemStorage *storage = cvCreateMemStorage();
    cvCanny(src,dst,50,200,3);
    
    cvCvtColor(dst,color_dst,CV_GRAY2BGR);
    
    CvSeq *circles = 0;
    int i = 0;
    circles = cvHoughCircles(dst, storage, CV_HOUGH_GRADIENT, dst->width / 10, 100);
    for (i = 0; i < circles->total; i++) {
         float* p = (float*)cvGetSeqElem(circles, i);
        CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));//圆心坐标（p（0），p（1））
        cvCircle(color_dst, pt, cvRound(p[2]),CV_RGB(255,0,0), 3);
    }
    
    cvNamedWindow("circle");
    cvShowImage("circle",color_dst);
    
    cvWaitKey(0);
};

// 获取指针
void Picture::getPointer() {
    
    IplImage *src = cvLoadImage("/Users/admin/Desktop/graduation/pic/meter1.jpeg",0);
    IplImage *dst = cvCreateImage(cvGetSize(src),8,1);
    IplImage *color_dst = cvCreateImage(cvGetSize(src),8,3);
    CvMemStorage *storage = cvCreateMemStorage();
    CvSeq *lines = 0;
    int i ;
    cvCanny(src,dst,50,200,3);
    
    cvCvtColor(dst,color_dst,CV_GRAY2BGR);
    
#if 0
    lines = cvHoughLines2(dst,storage,CV_HOUGH_STANDARD,1,CV_PI/180,150,0,0);
    
    for (i=0;i<lines->total;i++)
    {
        float *line = (float *)cvGetSeqElem(lines,i);
        float rho = line[0];
        float theta = line[1];
        CvPoint pt1,pt2;
        double a = cos(theta);
        double b = sin(theta);
        if (fabs(a)<0.001)
        {
            pt1.x = pt2.x = cvRound(rho);
            pt1.y = 0;
            pt2.y = color_dst->height;
        }
        else if (fabs(b)<0.001)
        {
            pt1.y = pt2.y = cvRound(rho);
            pt1.x = 0;
            pt2.x = color_dst->width;
        }
        else
        {
            pt1.x = 0;
            pt1.y = cvRound(rho/b);
            pt2.x = cvRound(rho/a);
            pt2.y = 0;
        }
        
//        cvLine(color_dst,pt1,pt2,CV_RGB(255,0,0),1,8);
    }
#else
    
    
    lines = cvHoughLines2(dst,storage,CV_HOUGH_PROBABILISTIC,1,CV_PI/180,80,30,5);
    
    for (i=0;i<lines->total;i++)
    {
        // 得到的直线转化成CvPoint型数据，该型数据包含点的横竖坐标；
        CvPoint *line = (CvPoint *)cvGetSeqElem(lines,i);
        // 画线函数 分别为直线的起始端点和结束端点
//        cvLine(color_dst,line[0],line[1],CV_RGB(255,0,0),1,CV_AA);
    }
#endif
    // 存放找到的线段长度的数组
    double maxLength = 0;
    CvPoint *maxLengthline = 0;
    
    for (i = 0; i < lines->total; i++) {
        CvPoint *line = (CvPoint *)cvGetSeqElem(lines,i);
    
//        std::cout <<i << ":[ [" << line[0].x<< "," << line[0].y << "]"<< ",[" << line[1].x << "," << line[1].y<< "] ]"<< std::endl;
        
        int startX = line[0].x;
        int startY = line[0].y;
        int endX = line[1].x;
        int endY = line[1].y;
        double length = sqrt(pow(startX - endX, 2) + pow(startY - endY, 2));
        if (length > maxLength) {
            maxLength = length;
            maxLengthline = line;
        }
//        std::cout << "length = " << length << std::endl;

    }
    std::cout << "maxLength = " << maxLength << std::endl;
    
    this->pointer = maxLengthline;
    cvLine(color_dst,maxLengthline[0],maxLengthline[1],CV_RGB(255,0,0),1,CV_AA);
    
    int pointX, pointY;
    pointX = static_cast<int>( this->centerPoint.x);
    pointY = static_cast<int>( this->centerPoint.y);
    
    // 圆心坐标
    CvPoint centerPoint = CvPoint(pointX, pointY);
    
    // 画出圆心
    cvCircle( color_dst, centerPoint, 2, CV_RGB(255,0,0),
             -1, 8, 0 );
    
    
    cvNamedWindow("Source");
    cvShowImage("Source",src);
    
    cvNamedWindow("Hough");
    cvShowImage("Hough",color_dst);
    
    cvWaitKey(0);
    
    cvReleaseImage(&src);
    cvReleaseImage(&dst);
    cvReleaseImage(&color_dst);
    cvReleaseMemStorage(&storage);
    
    cvDestroyAllWindows();
}



















//  以下为辅助函数
