#ifndef VISION_MINI_PROJECT_VIS_H
#define VISION_MINI_PROJECT_VIS_H

#include <opencv2/core/core.hpp>

namespace vis {
    // Analytics
    cv::Mat calcHist(const cv::Mat &img, bool accumulate = false);

    cv::Mat histImage(const cv::Mat &hist, double gamma = 1);

    void showHist(const cv::String &title, const cv::Mat &hist, double gamma = 1);

    cv::Mat fft2d(const cv::Mat &img);

    cv::Mat ifft2d(const cv::Mat &fft2d);

    // Intensity transformations
    cv::Mat brighten(const cv::Mat &src, int intensity);

    cv::Mat histogramStretch(const cv::Mat &src, uchar mi, uchar ma);

    cv::Mat histogramStretch(const cv::Mat &src, float threshold = 0);

    // Filters
    cv::Mat maxFilter(const cv::Mat &src, unsigned int kernelSize);

    cv::Mat meanMinMax(const cv::Mat &src, unsigned int kernelSize, uchar min, uchar max);

    cv::Mat medianMinMax(const cv::Mat &src, unsigned int kernelSize, uchar min, uchar max);

    cv::Mat adaptiveMedian(const cv::Mat &img, unsigned int MaxKernelSize);

    cv::Mat adaptiveNoiseReduction(const cv::Mat &src, unsigned int kernelSize, double var);

    // Utils
    int extractKernel(const cv::Mat &src, std::vector<uchar> &kernel, int x, int y, unsigned int kernelSize);

    std::vector<uchar> extractKernel(const cv::Mat &src, int x, int y, unsigned int kernelSize);

    std::vector<uchar> extractKernel(const cv::Mat &src, const cv::Point &center, unsigned int kernelSize);

    cv::Mat cropBorder(const cv::Mat &src, int borderWidth);

    cv::Mat scale(const cv::Mat &src, double scale);

    void show(const cv::String &title, const cv::Mat &src, double scale = 1);

    cv::Mat fft2dMagnitude(const cv::Mat &fft2d);

    void flipQuadrants(cv::Mat &src);

    std::pair<double, double> meanvar(const uchar *begin, const uchar *end);
}

#endif //VISION_MINI_PROJECT_VIS_H
