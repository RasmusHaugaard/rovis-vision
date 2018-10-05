#include "vis.h"
#include <iostream>

cv::Mat vis::brighten(const cv::Mat &src, const int intensity) {
    int m = src.rows, n = src.cols;
    cv::Mat dst = cv::Mat(m, n, CV_8UC1);

    uchar *srcPt = src.data;
    uchar *dstPt = dst.data;

    for (int i = 0; i < m * n; i++) {
        int val = srcPt[i] + intensity;
        dstPt[i] = cv::saturate_cast<uchar>(val);
    }

    return dst;
}

cv::Mat vis::histogramStretch(const cv::Mat &src, uchar mi, uchar ma) {
    int m = src.rows, n = src.cols;
    cv::Mat dst = cv::Mat(m, n, CV_8UC1);
    cv::Mat lut = cv::Mat(256, 1, CV_8UC1);

    for (int i = 0; i < 256; i++) {
        if (i <= mi) {
            lut.data[i] = 0;
        } else if (i < ma) {
            float s = (float) (i - mi) / (ma - mi);
            lut.data[i] = (uchar) round(s * 255);
        } else {
            lut.data[i] = 255;
        }
    }

    cv::LUT(src, lut, dst);

    return dst;
}

cv::Mat vis::histogramStretch(const cv::Mat &src, float threshold) {
    int m = src.rows, n = src.cols;

    int t = (int) round(m * n * threshold);

    cv::Mat hist = vis::calcHist(src);
    for (int i = 1; i < 256; i++)
        hist.at<float>(i) += hist.at<float>(i - 1);

    uchar mi = 1, ma = 254;

    for (int i = 0; i < 256; i++) {
        if (hist.at<float>(i) > t) {
            mi = (uchar) i;
            break;
        }
    }

    for (int i = 255; i >= 0; i--) {
        if (hist.at<float>(i) < m * n - t) {
            ma = (uchar) i;
            break;
        }
    }
    return vis::histogramStretch(src, mi, ma);
}