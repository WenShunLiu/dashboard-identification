//
//  main.cpp
//  dashboard-identification
//
//  Created by LWS on 2018/4/11.
//  Copyright © 2018年 LWS. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <string>
#include "Picture.hpp"
using namespace cv;

int main(int argc, const char * argv[]) {
//    std::cout<< "参数1" << argc << std::endl;
//    std::cout<< "参数2" << argv[0]<< "," << argv[1] << std::endl;
    
//    String flod = argv[1];
//    String demoSrc = flod + "/index.jpeg";
//    String zerosrc = flod+"/zero.png";
//    String fullsrc = flod+"/full.png";
//    String tmpsrc = flod+"/index2.jpeg";
    
    String flod = "meter4";
    String demoSrc = "/Users/admin/Desktop/graduation/pic/" + flod + "/index.jpeg";
    String zerosrc = "/Users/admin/Desktop/graduation/pic/"+flod+"/zero.png";
    String fullsrc = "/Users/admin/Desktop/graduation/pic/"+flod+"/full.png";
    String tmpsrc = "/Users/admin/Desktop/graduation/pic/"+flod+"/index2.jpeg";

    Mat imgSrc = imread(demoSrc);
    
    if(imgSrc.empty()) {
        std::cout << "图片打开失败" << std::endl;
        return 0;
    }
    double Range = 80;
    resize(imgSrc, imgSrc, Size(imgSrc.cols*2,imgSrc.rows*2),0,0,INTER_LINEAR);
    Picture demoPic(imgSrc,zerosrc,fullsrc, tmpsrc, Range);
    
    demoPic.showFirstPic();
    
    demoPic.showPanePic();
    
    CvPoint centerPoint = demoPic.getcenterPoint();
    
    std::cout << "圆心坐标为：[" << centerPoint.x << ", " << centerPoint.y<< "]" << std::endl;
    
    demoPic.getPointer();
    
    demoPic.getScale();
    demoPic.getFullAngle();
    demoPic.getCurrentAngle();
    double data = demoPic.getData();
    return 0;
};











 // 下面为初始例子
/*
void open(IplImage* src);

int main(int argc, const char * argv[]) {
    IplImage *src;
    src=cvLoadImage("/Users/admin/Desktop/graduation/pic/meter2.jpeg",1);//原图
//    open(src);
    
    Mat srcImage = imread("/Users/admin/Desktop/graduation/pic/meter2.jpeg");
//    imshow("Original Image", srcImage);
    
    Mat grayImage,edge;
    //转换为灰度图
    cvtColor(srcImage, grayImage, CV_BGR2GRAY);
    //降噪
    blur(grayImage, edge, Size(3,3));
    //运行Canny算子，3为threshold1，9为threshold2
    Canny(edge, edge, 3, 9);
    
    imshow("After canny", edge);
    waitKey(0);
    return 0;
}

void open(IplImage* src) {
    cvNamedWindow("admin",1);
    cvShowImage("admin",src);
    cvWaitKey(0);
    cvDestroyWindow("admin");
    cvReleaseImage(&src);
}
 */
