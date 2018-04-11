//
//  Picture.cpp
//  dashboard-identification
//
//  Created by LWS on 2018/4/11.
//  Copyright © 2018年 LWS. All rights reserved.
//

#include "Picture.hpp"
Picture::Picture(String src) {
    this->src = src;
};

void Picture::showPic() {
    Mat srcImage = imread(this->src);
    imshow("Original Image", srcImage);
    waitKey(0);
};
