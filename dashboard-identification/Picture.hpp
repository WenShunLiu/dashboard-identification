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
#include <string>
using namespace cv;

class Picture {
private:
    String src; // 路径
public:
    Picture(String src); // 构造函数
    void showPic(); // 展示图片
    void openPic(); // 打开图片(貌似和showPic是一样的)
};

#endif /* Picture_hpp */
