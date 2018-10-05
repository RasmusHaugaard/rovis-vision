#include "vis.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main()
{
    cv::Mat src = cv::imread("../images/Image1.png", cv::IMREAD_GRAYSCALE);

    vis::show("src", src, 0.3);

    cv::Mat hist_src = vis::calcHist(src);
    vis::showHist("src hist", hist_src);

    float zeros = hist_src.at<float>(0);
    std::cout << "zeros: " << (zeros / (src.cols * src.rows)) << std::endl;

    cv::Mat median = vis::medianMinMax(src, 5, 1, 255);
    vis::show("Median", median, 0.3);

    cv::Mat hist_median = vis::calcHist(median);
    vis::showHist("median hist", hist_median);

    cv::Mat median_eqHist;
    cv::equalizeHist(median, median_eqHist);
    vis::show("after eq hist", median_eqHist, 0.3);

    cv::Mat hist_median_eqHist = vis::calcHist(median_eqHist);
    vis::showHist("median hist eq", hist_median_eqHist);

    cv::waitKey(0);
    return 0;
}
