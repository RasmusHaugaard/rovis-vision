#include "vis.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cstdlib>

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

void vis::showPartialHist(const cv::String& title, const cv::Mat& hist, double gamma, int imgCount)
{

    cv::Mat h = hist.clone();
    int x = 0, y = 0;

    std::vector<cv::Mat> imgCuts;
    std::cout << h.cols << " , " << h.rows << std::endl;
    for (int i = 0; i < imgCount; i++)
    {
        x = (rand() % h.cols-200);
        y = (rand() % h.rows-200);
        std::cout << x << " , " << y << std::endl;
        imgCuts.push_back(cv::Mat(cv::Size(h.rows, h.cols), CV_8UC1, cv::Scalar(0)));
        for (int r = y; r < 200+y; r++)
            for (int c = x; c < 200+x; c++)
            {
                if(r >= h.rows || c >= h.cols)
                    break;
                imgCuts[i].at<uchar>(r,c) = h.at<uchar>(r,c);
            }
        cv::Mat tempHist = calcHist(imgCuts[i]);
        showHist("Partial hist " + std::to_string(i), tempHist, gamma);
    }
}

