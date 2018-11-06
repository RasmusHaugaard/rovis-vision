#include "vis.h"
#include "rovis1.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main() {
    cv::Mat src = cv::imread("../images/Image1.png", cv::IMREAD_GRAYSCALE);

    rovis1::imageAnalysis("1", "src", src);

    float zeros = vis::calcHist(src).at<float>(0);
    std::cout << "zeros ratio: " << (zeros / (src.cols * src.rows)) << std::endl;
    // Zeros ratio; 0.6 > 0.5, so simple median filter wont work

    // Instead, ignore the zeros and do a median filter of the remaining values
    auto median = vis::medianMinMax(src, 5, 1, 255);
    rovis1::imageAnalysis("1", "median", median);

    // clean up a little
    auto adap = vis::adaptiveNoiseReduction(median, 11, rovis1::calcEv1Var(median));
    rovis1::imageAnalysis("1", "adap", adap);

    // try histogram equalization
    cv::Mat eqhist;
    cv::equalizeHist(adap, eqhist);
    rovis1::imageAnalysis("1", "eqhist", eqhist);

    // and stretch histogram to increase contrast
    auto stretched = vis::histogramStretch(adap);
    rovis1::imageAnalysis("1", "stretched", stretched);

    return 0;
}
