#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>
#include "vis.h"

cv::Mat vis::cropBorder(const cv::Mat &src, int borderWidth) {
    cv::Rect roi(
            borderWidth,
            borderWidth,
            src.cols - borderWidth * 2,
            src.rows - borderWidth * 2
    );

    return src(roi);
}

cv::Mat vis::scale(const cv::Mat &src, double scale) {
    int width = (int) round(src.cols * scale);
    int height = (int) round(src.rows * scale);
    cv::Mat dst(height, width, CV_8UC1);
    cv::resize(src, dst, dst.size(), 0, 0, CV_INTER_CUBIC);
    return dst;
}

void vis::show(const cv::String& title, const cv::Mat &src, double scale){
    if (scale == 1){
        cv::imshow(title, src);
        return;
    }
    auto scaled_image = vis::scale(src, scale);
    cv::imshow(title, scaled_image);
}
