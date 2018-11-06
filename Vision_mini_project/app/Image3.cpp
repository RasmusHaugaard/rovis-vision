#include "vis.h"
#include "rovis1.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main() {
    cv::Mat src = cv::imread("../images/Image3.png", cv::IMREAD_GRAYSCALE);

    rovis1::imageAnalysis("3", "src", src);

    // clean up
    auto adap = vis::adaptiveNoiseReduction(src, 13, rovis1::calcEv1Var(src));
    rovis1::imageAnalysis("3", "adap", adap);

    // and stretch histogram to increase contrast
    auto stretched = vis::histogramStretch(adap, 0.001);
    rovis1::imageAnalysis("3", "stretched", stretched);

    return 0;
}
