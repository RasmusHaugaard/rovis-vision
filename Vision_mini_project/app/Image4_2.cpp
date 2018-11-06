#include "vis.h"
#include "rovis1.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

void butterworth(cv::Mat img, double x, double y, double d, int n) {
    for (int v = 0; v < img.rows; v++) {
        for (int u = 0; u < img.cols; u++) {
            double du = u - x, dv = v - y;
            double f = 1.0 / (1 + pow(sqrt(du * du + dv * dv) / d, -2 * n));
            img.at<float>(v, u * 2) *= f;
            img.at<float>(v, u * 2 + 1) *= f;
        }
    }
}


int main() {
    cv::Mat src = cv::imread("../images/Image4_2.png", cv::IMREAD_GRAYSCALE);
    rovis1::imageAnalysis("4_2", "src", src);

    cv::Mat fft2d = vis::fft2d(src);
    auto fft2dmag = vis::fft2dMagnitude(fft2d);
    cv::imwrite("../results/4_2/src_freq_mag.png", vis::scale(fft2dmag, 0.2) * 255);

    // radius found experimentally
    double r = fft2d.cols / 4.0 * 1.05;

    cv::Point centerPoint(fft2d.cols / 2, fft2d.rows / 2);

    for (int i = 0; i < 8; i++) {
        double angle = (2 * M_PI * i) / 8.0;
        int x = int(round(cos(angle) * r + centerPoint.x));
        int y = int(round(sin(angle) * r + centerPoint.y));
        butterworth(fft2d, x, y, 40, 2);
    }

    fft2dmag = vis::fft2dMagnitude(fft2d);
    cv::imwrite("../results/4_2/res_freq_mag.png", vis::scale(fft2dmag, 0.2) * 255);

    cv::Mat res = vis::ifft2d(fft2d);
    rovis1::imageAnalysis("4_2", "res", res);

    return 0;
}
