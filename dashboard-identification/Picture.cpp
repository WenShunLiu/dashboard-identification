//
//  Picture.cpp
//  dashboard-identification
//
//  Created by LWS on 2018/4/11.
//  Copyright © 2018年 LWS. All rights reserved.
//

#include "Picture.hpp"

Picture::Picture(Mat firstPic) {
    this->firstPic = firstPic;
};

void Picture::showFirstPic() {
    imshow("Original Image", this->firstPic);
    waitKey(0);
};

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

void Picture::showPanePic() {
    this->getPanePic();
    if(this->panePic.empty()) {
        std::cout << "获取表盘失败" << std::endl;
        return;
    }
    imshow("Original Image", this->panePic);
    waitKey(0);
};
Mat Picture::getcenterPoint() {
    Mat demo;
    std::vector<std::vector<Point>>contours;
    std::vector<Vec4i>hierarchy;
    //查找出所有的圆边界
    findContours(this->panePic, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    int index = 0;
    for (; index >= 0; index = hierarchy[index][0])
    {
        Scalar color(rand() & 255, rand() & 255, rand() & 255);
        drawContours(this->firstPic, contours, index, color, CV_FILLED, 8, hierarchy);
    }
    //标准圆在图片上一般是椭圆，所以采用OpenCV中拟合椭圆的方法求解中心
    Mat pointsf;
    Mat(contours[0]).convertTo(pointsf, CV_32F);
    RotatedRect box = fitEllipse(pointsf);
    std::cout << box.center << std::endl << "liuwenshun" << std::endl;
    return demo;
};
