#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
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

void vis::show(const cv::String &title, const cv::Mat &src, double scale) {
    if (scale == 1) {
        cv::imshow(title, src);
        return;
    }
    auto scaled_image = vis::scale(src, scale);
    cv::namedWindow(title);
    cv::imshow(title, scaled_image);
}

void vis::flipQuadrants(cv::Mat &src) {
    int cx = src.cols / 2;
    int cy = src.rows / 2;

    cv::Mat q0(src, cv::Rect(0, 0, cx, cy)); // Top-Left
    cv::Mat q1(src, cv::Rect(cx, 0, cx, cy)); // Top-Right
    cv::Mat q2(src, cv::Rect(0, cy, cx, cy)); // Bottom-Left
    cv::Mat q3(src, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}