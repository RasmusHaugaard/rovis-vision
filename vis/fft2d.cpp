#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.hpp>
#include "vis.h"
#include <iostream>

cv::Mat vis::fft2d(const cv::Mat &src) {
    cv::Mat padded;
    cv::copyMakeBorder(src, padded, 0, src.rows, 0, src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat cmplx;
    cv::merge(planes, 2, cmplx);

    cv::dft(cmplx, cmplx);
    vis::flipQuadrants(cmplx);

    return cmplx;
}

cv::Mat vis::ifft2d(const cv::Mat &fft2d) {
    cv::Mat cmplx = fft2d.clone();
    vis::flipQuadrants(cmplx);

    cv::Mat img;
    cv::dft(cmplx, img, cv::DFT_INVERSE | cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
    img.convertTo(img, CV_8U);

    return img(cv::Rect(0, 0, cmplx.cols / 2, cmplx.rows / 2));
}

cv::Mat vis::fft2dMagnitude(const cv::Mat &fft2d) {
    // compute the magnitude
    // => sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    cv::Mat res;
    cv::Mat planes[2];
    cv::split(fft2d, planes); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::magnitude(planes[0], planes[1], res);

    // log representation
    res += cv::Scalar::all(1);
    cv::log(res, res);
    cv::normalize(res, res, 0, 1, CV_MINMAX);
    return res;
}