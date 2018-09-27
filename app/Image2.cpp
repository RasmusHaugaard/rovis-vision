#include "vis.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main() {
    cv::Mat src = cv::imread("../images/Image2.png", cv::IMREAD_GRAYSCALE);

    vis::show("src", src, 0.3);

    cv::Mat hist_src = vis::calcHist(src);
    vis::showHist("src hist", hist_src);

    float zeros = hist_src.at<float>(0);
    std::cout << "zeros: " << (zeros / (src.cols * src.rows)) << std::endl;

    float ones = hist_src.at<float>(255);
    std::cout << "ones: " << (ones / (src.cols * src.rows)) << std::endl;

    cv::Mat median;
    for (int k : {5}) {
        auto text = "Median " + std::to_string(k);
        median = vis::medianMinMax(src, k, 1, 254);
        vis::show(text, median, 0.3);
        cv::Mat hist = vis::calcHist(median);
        vis::showHist(text + " hist", hist);
    }

    cv::Mat res = vis::histogramStretch(median, 0.001);
    vis::show("res", res, 0.3);
    vis::showHist("res hist", vis::calcHist(res));

    cv::waitKey(0);
    return 0;
}
