#include <iostream>
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


cv::Mat vis::adaptiveMedian(const cv::Mat &img, unsigned int MaxKernelSize)
{
    cv::Mat dst(img.rows, img.cols, CV_8UC1);
    int kernelSize = 3;
    bool decreaseY;

    std::vector<uchar> vals;
    int k = kernelSize / 2;
    for (int y = 1; y < img.rows - 1; y++)
    {
        for (int x = 1; x < img.cols - 1; x++)
        {
            int i = 0;
            vals.clear();
            decreaseY = false;
            for (int yy = y - k; yy < y + k; yy++)
            {
                for (int xx = x - k; xx < x + k; xx++)
                {
                    vals.push_back(img.at<uchar>(yy, xx));
                    i++;
                }
            }
            std::sort(vals.begin(), vals.begin() + i);
            //std::cout << vals.size() << std::endl;
            //std::cout << "X: " << x << "   y: " << y << "   Kernelsize: " << kernelSize << "   Val: " << static_cast<int>(vals[i/2]) << std::endl;

            if (!(vals[i/2] == 0 || vals[i/2] == 255))
            {
                dst.at<uchar>(y, x) = vals[i / 2];
                //std::cout << "Kernel Size: " << kernelSize << "   Pixel value: " << static_cast<int>(vals[i/2]) << std::endl;
                kernelSize = 3;
            }
            else if(kernelSize == MaxKernelSize)
            {
                dst.at<uchar>(y, x) = vals[i / 2];
                //std::cout << "Kernel Size: " << kernelSize << "   Pixel value: " << static_cast<int>(vals[i/2]) << std::endl;
                kernelSize = 3;
            }
            else
            {
                //std::cout << "Size++:  Kernel Size: " << kernelSize << "   Pixel value: " << static_cast<int>(vals[i/2]) << std::endl;
                if (!(x < k || x > img.cols - k || y < k || y > img.rows - k))
                {
                    kernelSize = kernelSize+2;
                    x--;
                    decreaseY = true;
                }
            }
            k = kernelSize / 2;
        }
        if (decreaseY)
            y--;
    }

    dst = vis::cropBorder(dst, k);

    return dst;
}
