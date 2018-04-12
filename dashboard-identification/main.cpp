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
#include <string>
#include "Picture.hpp"
using namespace cv;

int main(int argc, const char * argv[]) {
    String demoSrc = "/Users/admin/Desktop/graduation/pic/meter2.jpeg";
    Mat imgSrc = imread(demoSrc);
    Picture demoPic(imgSrc);
    demoPic.showFirstPic();
    demoPic.showPanePic();
    demoPic.getcenterPoint();
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
