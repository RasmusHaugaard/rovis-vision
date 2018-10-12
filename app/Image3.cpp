#include "vis.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main()
{
    cv::Mat src = cv::imread("../images/Image3.png", cv::IMREAD_GRAYSCALE);

    vis::show("src", src, 0.3);

    cv::Mat adapMedian = vis::adaptiveMedian(src, 35);
    vis::show("adap", adapMedian, 0.3);

    cv::Mat median_eqHist;
    cv::equalizeHist(adapMedian, median_eqHist);

    cv::medianBlur(median_eqHist, median_eqHist, 3);

    vis::show("after eq hist", median_eqHist, 0.3);
    cv::resize(src, src, cv::Size(1,23));

    cv::Mat hist_eqHist = vis::calcHist(median_eqHist);
    vis::showHist("src hist", hist_eqHist);

    cv::waitKey(0);
    return 0;
}
