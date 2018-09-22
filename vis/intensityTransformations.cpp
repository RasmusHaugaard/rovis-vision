#include "vis.h"

cv::Mat vis::brighten(const cv::Mat& src, const int intensity)
{
    cv::Mat dst = cv::Mat(src.rows, src.cols, CV_8UC1);

    uchar* srcPt = src.data;
    uchar* dstPt = dst.data;

    for (int i = 0; i < src.rows * src.cols; i++){
        int val = srcPt[i] + intensity;
        dstPt[i] = cv::saturate_cast<uchar>(val);
    }

    return dst;
}