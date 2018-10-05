#ifndef VISION_MINI_PROJECT_VIS_H
#define VISION_MINI_PROJECT_VIS_H

#include <opencv2/core/core.hpp>

namespace vis {
    // Analytics
    cv::Mat calcHist(const cv::Mat &img, bool accumulate = false);

    void showHist(const cv::String &title, const cv::Mat &hist, double gamma = 1);

    cv::Mat fft2d(const cv::Mat &img);

    cv::Mat ifft2d(const cv::Mat &fft2d);

    // Intensity transformations
    cv::Mat brighten(const cv::Mat &src, int intensity);

    cv::Mat histogramStretch(const cv::Mat& src, uchar mi, uchar ma);

    cv::Mat histogramStretch(const cv::Mat& src, float threshold = 0);

    // Filters
    cv::Mat medianMinMax(const cv::Mat &img, unsigned int kernelSize, uchar min, uchar max);

    // Utils
    cv::Mat cropBorder(const cv::Mat &src, int borderWidth);

    cv::Mat scale(const cv::Mat &src, double scale);

    void show(const cv::String &title, const cv::Mat &src, double scale = 1);

    void showfft2dMagnitude(const cv::String &title, const cv::Mat &fft2d, double scale = 1);

    void flipQuadrants(cv::Mat &src);
}

#endif //VISION_MINI_PROJECT_VIS_H
