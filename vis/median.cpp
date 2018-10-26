#include <iostream>
#include "vis.h"

cv::Mat vis::meanMinMax(const cv::Mat &src, unsigned int kernelSize, uchar min, uchar max) {
    cv::Mat dst(src.rows, src.cols, CV_8UC1);

    std::vector<uchar> kernel(kernelSize * kernelSize);
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            int size = vis::extractKernel(src, kernel, x, y, kernelSize);
            int validSize = 0, sum = 0;
            for (int j = 0; j < size; j++)
                if (min <= kernel[j] && kernel[j] <= max)
                    sum += kernel[j], validSize++;
            dst.at<uchar>(y, x) = uchar(round(double(sum) / validSize));
        }
    }

    return dst;
}

cv::Mat vis::medianMinMax(const cv::Mat &src, unsigned int kernelSize, uchar min, uchar max) {
    cv::Mat dst(src.rows, src.cols, CV_8UC1);

    std::vector<uchar> kernel(kernelSize * kernelSize);
    std::vector<uchar> vals(kernelSize * kernelSize);
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            int size = vis::extractKernel(src, kernel, x, y, kernelSize);
            int validSize = 0;
            for (int j = 0; j < size; j++)
                if (min <= kernel[j] && kernel[j] <= max)
                    vals[validSize++] = kernel[j];
            std::sort(vals.begin(), vals.begin() + validSize);
            dst.at<uchar>(y, x) = vals[validSize / 2];
        }
    }

    return dst;
}

cv::Mat vis::maxFilter(const cv::Mat &src, unsigned int kernelSize) {
    cv::Mat dst(src.rows, src.cols, CV_8UC1);

    std::vector<uchar> kernel(kernelSize * kernelSize);
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            int size = vis::extractKernel(src, kernel, x, y, kernelSize);
            dst.at<uchar>(y, x) = *std::max_element(kernel.begin(), kernel.begin() + size);
        }
    }

    return dst;
}


cv::Mat vis::adaptiveMedian(const cv::Mat &img, unsigned int maxKernelSize) {
    cv::Mat dst(img.rows, img.cols, CV_8UC1);
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            uchar med = 0;
            for (unsigned int kSize = 3; kSize <= maxKernelSize; kSize += 2) {
                auto kernel = vis::extractKernel(img, x, y, kSize);
                std::sort(kernel.begin(), kernel.end());
                uchar min = kernel[0];
                uchar max = kernel[kernel.size() - 1];
                med = kernel[kernel.size() / 2];
                if (!(min == med || med == max)) break;
            }
            dst.at<uchar>(y, x) = med;
        }
    }
    return dst;
}
