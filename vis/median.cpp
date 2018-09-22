#include "vis.h"

cv::Mat vis::medianMinMax(const cv::Mat &img, unsigned int kernelSize, uchar min, uchar max){
    cv::Mat dst(img.rows, img.cols, CV_8UC1);

    std::vector<uchar> vals(kernelSize * kernelSize);
    const int kernelArm = kernelSize / 2;
    for (int y = kernelArm; y < img.rows - kernelArm; y++){
        for (int x = kernelArm; x < img.cols - kernelArm; x++){
            int i = 0;
            for (int yy = y - kernelArm; yy < y + kernelArm; yy++){
                for (int xx = x - kernelArm; xx < x + kernelArm; xx++){
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

    dst = vis::cropBorder(dst, kernelArm);

    return dst;
}
