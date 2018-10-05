#include "vis.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main()
{
    cv::Mat src = cv::imread("../images/Image3.png", cv::IMREAD_GRAYSCALE);

    vis::show("src", src, 0.3);

    cv::Mat hist_src = vis::calcHist(src);
    vis::showHist("src hist", hist_src);

    cv::Mat median = vis::medianMinMax(src, 7, 0, 254);
    vis::show("Median", median, 0.3);

    cv::Mat hist_median = vis::calcHist(median);
    vis::showHist("median hist", hist_median);

    cv::Mat median_eqHist;
    cv::equalizeHist(median, median_eqHist);

    cv::medianBlur(median_eqHist, median_eqHist, 3);

    vis::show("after eq hist", median_eqHist, 0.3);
    cv::resize(src, src, cv::Size(1,23));

    cv::Mat hist_eqHist = vis::calcHist(median_eqHist);
    vis::showHist("src hist", hist_eqHist);

    cv::waitKey(0);
    return 0;
}
