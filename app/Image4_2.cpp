#include "vis.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main() {
    const float scale = 0.2;
    cv::Mat src = cv::imread("../images/Image4_2.png", cv::IMREAD_GRAYSCALE);
    vis::show("src", src, scale);

    cv::Mat fft2d = vis::fft2d(src);
    vis::showfft2dMagnitude("src freq mag", fft2d, scale / 2);

    double r = fft2d.cols / 4.0 * 1.05;

    cv::Point centerPoint(fft2d.cols / 2, fft2d.rows / 2);

    for (int i = 0; i < 8; i++) {
        double angle = (2 * M_PI * i) / 8.0;
        int x = int(round(cos(angle) * r + centerPoint.x));
        int y = int(round(sin(angle) * r + centerPoint.y));
        cv::circle(fft2d, cv::Point(x, y), 20, cv::Scalar(0), -1);
    }

    vis::showfft2dMagnitude("filter applied to freq mag", fft2d, scale / 2);

    cv::Mat res = vis::ifft2d(fft2d);
    vis::show("res", res, scale);

    cv::waitKey(0);
    return 0;
}
