#include "vis.h"
#include "rovis1.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main() {
    cv::Mat src = cv::imread("../images/ImageOrig.png", cv::IMREAD_GRAYSCALE);

    rovis1::imageAnalysis("original", "src", src);

    return 0;
}
