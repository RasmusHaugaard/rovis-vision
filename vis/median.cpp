#include "vis.h"

cv::Mat vis::medianMinMax(const cv::Mat &img, unsigned int kernelSize, uchar min, uchar max) {
    cv::Mat dst(img.rows, img.cols, CV_8UC1);

    std::vector<uchar> vals(kernelSize * kernelSize);
    const int k = kernelSize / 2;
    for (int y = k; y < img.rows - k; y++) {
        for (int x = k; x < img.cols - k; x++) {
            int i = 0;
            vals[0] = 0;
            for (int yy = y - k; yy < y + k; yy++) {
                for (int xx = x - k; xx < x + k; xx++) {
                    uchar val = img.at<uchar>(yy, xx);
                    if (min <= val && val <= max) {
                        vals[i++] = val;
                    }
                }
            }
            std::sort(vals.begin(), vals.begin() + i);
            dst.at<uchar>(y, x) = vals[i / 2];
        }
    }

    dst = vis::cropBorder(dst, k);

    return dst;
}
