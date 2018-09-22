#ifndef VISION_MINI_PROJECT_VIS_H
#define VISION_MINI_PROJECT_VIS_H

#include <opencv2/core/core.hpp>

namespace vis {
    // Analytics
    cv::Mat calcHist(const cv::Mat& img);
    void showHist(const cv::String& title, const cv::Mat& hist, double gamma = 1);

    // Intensity transformations
    cv::Mat brighten(const cv::Mat& src, int intensity);

    // Filters
    cv::Mat medianMinMax(const cv::Mat &img, unsigned int kernelSize, uchar min, uchar max);

    // Utils
    cv::Mat cropBorder(const cv::Mat& src, int borderWidth);
    cv::Mat scale(const cv::Mat &src, double scale);
    void show(const cv::String& title, const cv::Mat &src, double scale = 1);
}

#endif //VISION_MINI_PROJECT_VIS_H
