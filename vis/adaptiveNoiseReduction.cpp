#include "vis.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <numeric>
#include <iostream>

cv::Mat vis::adaptiveNoiseReduction(const cv::Mat &src, unsigned int kernelSize, double nvar) {
    cv::Mat dst(src.rows, src.cols, CV_8UC1);

    auto noisePatch = vis::extractKernel(src, 1200, 1550, kernelSize * 2 + 1);
    nvar = vis::meanvar(&*noisePatch.begin(), &*noisePatch.end()).second;

    std::vector<uchar> kernel(kernelSize * kernelSize);
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            int size = vis::extractKernel(src, kernel, x, y, kernelSize);
            auto lmvar = vis::meanvar(&*kernel.begin(), &*(kernel.begin() + size));
            auto lvar = lmvar.second, lmean = lmvar.first;
            double r = 1;
            if (lvar > nvar) r = nvar / lvar;
            double g = src.at<uchar>(y, x);
            double fhat = r * lmean + (1.0 - r) * g;
            dst.at<uchar>(y, x) = (uchar) round(fhat);
        }
    }

    return dst;
}