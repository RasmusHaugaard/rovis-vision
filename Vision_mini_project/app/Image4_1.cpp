#include "vis.h"
#include "rovis1.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main() {
    const float scale = 0.3;
    cv::Mat src = cv::imread("../images/Image4_1.png", cv::IMREAD_GRAYSCALE);

    rovis1::imageAnalysis("4_1", "src", src);

    cv::Mat fft2d = vis::fft2d(src);
    auto fft2dmag = vis::fft2dMagnitude(fft2d);
    cv::imwrite("../results/4_1/src_freq_mag.png", vis::scale(fft2dmag, 0.2) * 255);

    cv::Point centerPoint(fft2d.cols / 2, fft2d.rows / 2);
    double r = fft2d.cols / 4.0 * 1.1;

    double angle1 = (0.75 * M_PI);
    double angle2 = (1.75 * M_PI);
    int x = int(round(cos(angle1) * r + centerPoint.x));
    int y = int(round(sin(angle1) * r + centerPoint.y));
    cv::circle(fft2d, cv::Point(x, y), 35, cv::Scalar(0), -1);
    x = int(round(cos(angle2) * r + centerPoint.x));
    y = int(round(sin(angle2) * r + centerPoint.y));
    cv::circle(fft2d, cv::Point(x, y), 35, cv::Scalar(0), -1);

    r = fft2d.cols / 10.5;
    angle1 = (0.25 * M_PI);
    angle2 = (1.25 * M_PI);
    x = int(round(cos(angle1) * r + centerPoint.x));
    y = int(round(sin(angle1) * r + centerPoint.y));
    cv::circle(fft2d, cv::Point(x, y), 25, cv::Scalar(0), -1);
    x = int(round(cos(angle2) * r + centerPoint.x));
    y = int(round(sin(angle2) * r + centerPoint.y));
    cv::circle(fft2d, cv::Point(x, y), 25, cv::Scalar(0), -1);

    fft2dmag = vis::fft2dMagnitude(fft2d);
    cv::imwrite("../results/4_1/res_freq_mag.png", vis::scale(fft2dmag, 0.2) * 255);

    cv::Mat res = vis::ifft2d(fft2d);
    rovis1::imageAnalysis("4_1", "res", res);

    return 0;
}
