#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "vis.h"
#include <numeric>

int vis::extractKernel(const cv::Mat &src, std::vector<uchar> &kernel, int x, int y, unsigned int kernelSize) {
    int k = kernelSize / 2;
    int x1 = x - k, x2 = x + k;
    int y1 = y - k, y2 = y + k;
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= src.cols) x2 = src.cols - 1;
    if (y2 >= src.rows) y2 = src.rows - 1;

    int i = 0;
    for (y = y1; y <= y2; y++)
        for (x = x1; x <= x2; x++, i++)
            kernel[i] = src.at<uchar>(y, x);

    return i;
}

std::vector<uchar> vis::extractKernel(const cv::Mat &src, int x, int y, unsigned int kernelSize) {
    std::vector<uchar> kernel(kernelSize * kernelSize);
    int i = vis::extractKernel(src, kernel, x, y, kernelSize);
    kernel.resize(i);
    return kernel;
}

std::vector<uchar> vis::extractKernel(const cv::Mat &src, const cv::Point &center, unsigned int kernelSize) {
    return extractKernel(src, center.x, center.y, kernelSize);
}

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
    int width = int(round(src.cols * scale));
    int height = int(round(src.rows * scale));
    cv::Mat dst(height, width, src.type());
    cv::resize(src, dst, dst.size(), 0, 0, CV_INTER_AREA);
    return dst;
}

void vis::show(const cv::String &title, const cv::Mat &src, double scale) {
    if (scale == 1) {
        cv::imshow(title, src);
    } else {
        cv::imshow(title, vis::scale(src, scale));
    }
}

void vis::flipQuadrants(cv::Mat &src) {
    int cx = src.cols / 2;
    int cy = src.rows / 2;

    cv::Mat q0(src, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(src, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(src, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(src, cv::Rect(cx, cy, cx, cy));

    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

std::pair<double, double> vis::meanvar(const uchar *begin, const uchar *end) {
    auto mean = std::accumulate(begin, end, 0.0) / (end - begin);
    double var = 0.0;
    std::for_each(begin, end, [&](uchar val) {
        var += (val - mean) * (val - mean);
    });
    return {mean, var / (end - begin)};
}



