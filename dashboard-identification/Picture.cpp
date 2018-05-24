//
//  Picture.cpp
//  dashboard-identification
//
//  Created by LWS on 2018/4/11.
//  Copyright © 2018年 LWS. All rights reserved.
//

#include "Picture.hpp"
CvPoint getScalePoint(Mat image,String src, int method);
CvPoint maxPoint(CvPoint pointone, CvPoint pointtwo, CvPoint center); // 求到圆心距离最长的点
double Angle(Point cen, Point first, Point second);


Picture::Picture(Mat firstPic, String zeroPicSrc,String fullPicSrc, double Range) {
    this->firstPic = firstPic;
    this->zeroPicSrc = zeroPicSrc;
    this->fullPicSrc = fullPicSrc;
    this->Range = Range;
};

// 显示原图
void Picture::showFirstPic() {
    imshow("orign pic", this->firstPic);
    waitKey(0);
};

// 获取盘身
void Picture::getPanePic() {
    
    IplImage src(this->firstPic);
    IplImage *dst = cvCreateImage(cvGetSize(&src),8,1);
    IplImage *color_dst = cvCreateImage(cvGetSize(&src),8,3);
    // 运行Canny算子 ,边缘提取
    cvCanny(&src,dst,50,200,3);
    // 转化为单通道
    cvCvtColor(dst,color_dst,CV_GRAY2BGR);
    this->panePic = dst;
    this->paneDraw =color_dst;
};

// 显示 盘身图片
void Picture::showPanePic() {
    this->getPanePic();
    cvNamedWindow("pan body");
    cvShowImage("pan body",this->panePic);
    waitKey(0);
};

// 获取圆心坐标
CvPoint Picture::getcenterPoint() {
    
    
    CvMemStorage *storage = cvCreateMemStorage();
    CvSeq *circles = 0;
    int i = 0;
    circles = cvHoughCircles(this->panePic, storage, CV_HOUGH_GRADIENT, this->panePic->width /15, 10);
    
    // 直径最大的圆
    float maxRadius = 0;
    
    // 直径最大的圆的圆心坐标
    CvPoint manRCircl = 0;
    for (i = 0; i < circles->total; i++) {
         float* p = (float*)cvGetSeqElem(circles, i);
        CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));//圆心坐标（p（0），p（1））
//        cvCircle(this->paneDraw, pt, cvRound(p[2]),CV_RGB(255,0,0), 3);
        if (cvRound(p[2]) > maxRadius) {
            maxRadius = cvRound(p[2]);
            manRCircl = pt;
        }
    }
    
    // 画出圆
    cvCircle(this->paneDraw, manRCircl, maxRadius,CV_RGB(255,0,0), 3);
    this->centerPoint = manRCircl;
    
    
    // 画出圆心
    cvCircle( this->paneDraw, manRCircl, 2, CV_RGB(255,0,0),-1, 8, 0 );
    cvNamedWindow("circle");
    cvShowImage("circle",this->paneDraw);
    
    cvWaitKey(0);
    return manRCircl;
};

// 获取指针
void Picture::getPointer() {
    
    CvSeq *lines = 0;
    int i ;
    CvMemStorage *storage = cvCreateMemStorage();
#if 0
    lines = cvHoughLines2(this->panePic,storage,CV_HOUGH_STANDARD,1,CV_PI/180,150,0,0);
    
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
            pt2.y = this->paneDraw->height;
        }
        else if (fabs(b)<0.001)
        {
            pt1.y = pt2.y = cvRound(rho);
            pt1.x = 0;
            pt2.x = this->paneDraw->width;
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
    
    
    lines = cvHoughLines2(this->panePic,storage,CV_HOUGH_PROBABILISTIC,1,CV_PI/180,80,30,5);
    
//    for (i=0;i<lines->total;i++)
//    {
        // 得到的直线转化成CvPoint型数据，该型数据包含点的横竖坐标；
//        CvPoint *line = (CvPoint *)cvGetSeqElem(lines,i);
        // 画线函数 分别为直线的起始端点和结束端点
//        cvLine(color_dst,line[0],line[1],CV_RGB(255,0,0),1,CV_AA);
//    }
#endif
    // 存放找到的线段长度的数组
    double maxLength = 0;
    CvPoint *maxLengthline = 0;
    
    for (i = 0; i < lines->total; i++) {
        CvPoint *line = (CvPoint *)cvGetSeqElem(lines,i);
        int startX = line[0].x;
        int startY = line[0].y;
        int endX = line[1].x;
        int endY = line[1].y;
        double length = sqrt(pow(startX - endX, 2) + pow(startY - endY, 2));
        if (length > maxLength) {
            maxLength = length;
            maxLengthline = line;
        }
    }
//    std::cout << "maxLength = " << maxLength << std::endl;
    
    cvLine(this->paneDraw,maxLengthline[0],maxLengthline[1],CV_RGB(255,0,0),1,CV_AA);
    
    
    
    CvPoint pointer = maxPoint(maxLengthline[0], maxLengthline[1], this->centerPoint);
    
    std::cout<< "指针: "<< pointer.x << "," << pointer.y<< std::endl;
    
    this->pointer = pointer;
    
    cvCircle(this->paneDraw, pointer, 4,CV_RGB(255,0,0),-1, 8, 0);
    cvNamedWindow("point");
    cvShowImage("point",this->paneDraw);
    
    cvWaitKey(0);
    
    cvReleaseMemStorage(&storage);
    
    cvDestroyAllWindows();
    
}

//得到0刻度
void Picture::getScale() {
    Mat image = this->firstPic;
    // 获取0刻度位置
    CvPoint zeroPoint =getScalePoint(image, this->zeroPicSrc, CV_TM_SQDIFF_NORMED);
    std::cout<< "zeroPoint: "<< zeroPoint.x << "," << zeroPoint.y << std::endl;
    this->zeroScale =zeroPoint;
    cvCircle(this->paneDraw, zeroPoint, 4,CV_RGB(255,0,0),-1, 8, 0);
    
    // 获取满偏刻度位置
    CvPoint fullPoint =getScalePoint(image, this->fullPicSrc, CV_TM_SQDIFF_NORMED);
    std::cout<< "fullPoint: "<< fullPoint.x << "," << fullPoint.y << std::endl;
    this->fullScale =fullPoint;
    cvCircle(this->paneDraw, fullPoint, 4,CV_RGB(255,0,0),-1, 8, 0);
    cvShowImage("scale", this->paneDraw);
    waitKey(0);
}

// 获取满偏角度
void Picture::getFullAngle() {
    CvPoint center = this->centerPoint;
    CvPoint zeroP = this->zeroScale;
    CvPoint fullP = this->fullScale;
    double a1 = Angle(center, zeroP, fullP);
    this->fullAngle = 360-a1;
}

// 获取当前指针偏移角度
void Picture::getCurrentAngle() {
    CvPoint center = this->centerPoint;
    CvPoint zeroP = this->zeroScale;
    CvPoint pointer = this->pointer;
    double a1 = Angle(center, zeroP, pointer);
    this->currentAngle = a1;
}

// 获取表盘读数
double Picture::getData() {
    double bili = (double) this->currentAngle/this->fullAngle;
    std::cout << "currentAngle: " << this->currentAngle << std::endl;
    std::cout << "fullAngle: " << this->fullAngle << std::endl;
    std::cout << "bili: " << bili << std::endl;
    double data = bili * this->Range;
    std::cout << "data: " << data << std::endl;
    this->data = data;
    
    
    cvLine(this->paneDraw,this->centerPoint,this->zeroScale,CV_RGB(255,0,0),1,CV_AA);
    cvLine(this->paneDraw,this->centerPoint,this->fullScale,CV_RGB(255,0,0),1,CV_AA);
    cvLine(this->paneDraw,this->centerPoint,this->pointer,CV_RGB(255,0,0),1,CV_AA);
    cvNamedWindow("data");
    cvShowImage("data",this->paneDraw);
    
    cvWaitKey(0);
    return data;
}

//  以下为辅助函数

// 模板匹配，获取0刻度和满偏刻度位置
CvPoint getScalePoint(Mat image, String src, int method) {
    Mat tepl = imread(src);
    resize(tepl, tepl, Size(tepl.cols/2,tepl.rows/2),0,0,INTER_LINEAR);
    CvPoint point;
    int result_cols =  image.cols - tepl.cols + 1;
    int result_rows = image.rows - tepl.rows + 1;
    
    Mat result = Mat( result_cols, result_rows, CV_32FC1 );
    matchTemplate( image, tepl, result, method );
    
    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    
    switch(method)
    {
        case CV_TM_SQDIFF:
            point = minLoc;
            //            return minVal / (tepl.cols * tepl.cols);
            break;
        case CV_TM_SQDIFF_NORMED:
            point = minLoc;
            //            return minVal;
            break;
        case CV_TM_CCORR:
        case CV_TM_CCOEFF:
            point = maxLoc;
            //            return maxVal / (tepl.cols * tepl.cols);
            break;
        case CV_TM_CCORR_NORMED:
        case CV_TM_CCOEFF_NORMED:
        default:
            point = maxLoc;
            //            return maxVal;
            break;
    }
//    rectangle(image, point, Point(point.x + tepl.cols, point.y + tepl.rows), Scalar(0, 255, 0), 2, 8, 0);
    return  CvPoint(point.x + tepl.cols/2, point.y + tepl.rows/2);
};


// 获取指针上离圆心最远的点
CvPoint maxPoint(CvPoint pointone, CvPoint pointtwo, CvPoint center) {
    double lengthone = pow(pointone.x-center.x, 2) + pow(pointone.y-center.y, 2);
    double lengthtwo = pow(pointtwo.x-center.x, 2) + pow(pointtwo.y-center.y, 2);
    return lengthone > lengthtwo ? pointone : pointtwo;
}






// 获取两条向量的角度cosABC=BA*CB/a*b,分子是向量相乘,AB=a,BC=b
double Angle(Point cen, Point first, Point second)
{
    Point newZerop = Point(first.x - cen.x, first.y-cen.y);
    Point newFullp = Point(second.x - cen.x, second.y-cen.y);
    double ma_x = first.x - cen.x;
    double ma_y = first.y - cen.y;
    double mb_x = second.x - cen.x;
    double mb_y = second.y - cen.y;
    double v1 = (ma_x * mb_x) + (ma_y * mb_y);
    double ma_val = sqrt(ma_x * ma_x + ma_y * ma_y);
    double mb_val = sqrt(mb_x * mb_x + mb_y * mb_y);
    double cosM = v1 / (ma_val * mb_val);
//    std::cout<<"cosM:"<<cosM<<std::endl;
//    if (cosM<0) {
//        
//    }
    double angleAMB = acos(cosM) * 180 / PI;
    
    return angleAMB;
}
