#include "vis.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat vis::calcHist(const cv::Mat &img, bool accumulate) {
    const int nBins = 256;
    const float range[] = {0, nBins};
    const float *histRange = {range};
    bool uniform = true;
    cv::Mat hist;

    cv::calcHist(&img, 1, nullptr, cv::Mat(), hist, 1, &nBins, &histRange, uniform, accumulate);

    return hist;
}

void vis::showHist(const cv::String& title, const cv::Mat& hist, double gamma) {
    const int nBins = hist.rows;
    int hist_w = nBins;
    int hist_h = nBins;

    cv::Mat h = hist.clone();

    cv::normalize(h, h, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 0; i < nBins; i++){
        h.at<float>(i) = (float) pow(h.at<float>(i), gamma);
    }

    cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(0));

    for (int i = 0; i < nBins; i++) {
        cv::line(
                histImage,
                cv::Point(i, hist_h),
                cv::Point(i, hist_h - cvRound(h.at<float>(i) * hist_h)),
                cv::Scalar(255)
        );
    }

    cv::imshow(title, histImage);
}

