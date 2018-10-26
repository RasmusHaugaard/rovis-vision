#include "vis.h"
#include "rovis1.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main() {
    cv::Mat src = cv::imread("../images/Image2.png", cv::IMREAD_GRAYSCALE);

    rovis1::imageAnalysis("2", "src", src);

    cv::Mat hist_src = vis::calcHist(src);
    float zeros = hist_src.at<float>(0);
    std::cout << "zeros: " << (zeros / (src.cols * src.rows)) << std::endl;
    float ones = hist_src.at<float>(255);
    std::cout << "ones: " << (ones / (src.cols * src.rows)) << std::endl;

    // since p(0) ~= 0.1, and p(1) ~= 0.3, an adaptive mean filter will do
    // medianMinMax actually works better, but we wanted to use the adaptive filter
    auto median = vis::adaptiveMedian(src, 11);
    //auto median = vis::medianMinMax(src, 5, 1, 254);
    rovis1::imageAnalysis("2", "median", median);

    // clean up a little
    auto adap = vis::adaptiveNoiseReduction(median, 11, rovis1::calcEv1Var(median));
    rovis1::imageAnalysis("2", "adap", adap);

    // and stretch histogram to increase contrast
    auto stretched = vis::histogramStretch(adap, 0.001);
    rovis1::imageAnalysis("2", "stretched", stretched);

    return 0;
}
