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
#include <typeinfo>
#include <highgui.h>
#include <math.h>
using namespace cv;

#define PI 3.14159265

class Picture {
private:
    Mat firstPic; // 图片
    Mat panePic; // 提取表盘区域后的图像
    Point_<float> centerPoint; // 圆心坐标
public:
    Picture(Mat firstPic); // 构造函数
    void showFirstPic(); // 展示原始图片
    void getPanePic(); // 获取表盘图片
    void showPanePic(); // 展示表盘
    Point_<float> getcenterPoint(); // 获取表盘圆心坐标
    void getPointer();
};

#endif /* Picture_hpp */
