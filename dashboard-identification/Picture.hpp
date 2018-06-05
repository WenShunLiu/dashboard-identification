//
//  Picture.hpp
//  dashboard-identification
//
//  Created by LWS on 2018/4/11.
//  Copyright © 2018年 LWS. All rights reserved.
//

#ifndef Picture_hpp
#define Picture_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

#include <typeinfo>
//#include <highgui.h>
#include <math.h>
using namespace cv;

#define PI 3.14159265;

class Picture {
private:
    Mat firstPic; // 图片
    String zeroPicSrc; // 0刻度图片路径
    String fullPicSrc; // 满偏图片路径
    String tmpPicSrc;
    IplImage *panePic; // 提取表盘区域后的图像
    IplImage *paneDraw; // 用于绘制的表盘
    double Range; // 量程
    CvPoint centerPoint; // 圆心坐标
    float radius; // 圆半径
    CvPoint pointer; //指针
    CvPoint zeroScale; // 0刻度位置
    CvPoint fullScale; // 满偏刻度位置
    double fullAngle; // 满偏角度
    double currentAngle; // 当前指针角度
    double data; //表盘读数
public:
    Picture(Mat firstPic,String zeroPicSrc,String fullPicSrc, String tmpPicSrc,double Range); // 构造函数
    void showFirstPic(); // 展示原始图片
    void getPanePic(); // 获取表盘图片
    void showPanePic(); // 展示表盘
    CvPoint getcenterPoint(); // 获取表盘圆心坐标
    void getPointer(); //获取指针
    void getScale(); // 得到0刻度
    void getFullAngle(); // 获取满偏角度
    void getCurrentAngle(); // 获取当前指针角度
    double getData(); // 获取表盘读数
};

#endif /* Picture_hpp */
