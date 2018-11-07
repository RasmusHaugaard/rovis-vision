#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

int main() {
    cv::RNG rng(12345);

    std::string imgPath = "../marker_thinline_easy/marker_thinline_";
    std::string fileType = ".png";
    std::vector<cv::Mat> imgSequence;
    cv::Mat imgTemp;

    for (int i = 1; i < 31; i++) {
        if (i < 10)
            imgTemp = cv::imread(imgPath + "0" + std::to_string(i) + fileType);
        else
            imgTemp = cv::imread(imgPath + std::to_string(i) + fileType);
        imgSequence.push_back(imgTemp);
    }

    cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("hough", cv::WINDOW_AUTOSIZE);

    int j = 0;
    while (true) {
        std::cout << "loop: " << j++ << std::endl;
        for (int i = 0; i < 30; i++) {

            // Gray
            cv::Mat gray;
            cv::cvtColor(imgSequence[i], gray, cv::COLOR_BGR2GRAY);

            // Canny edge
            cv::Mat canny;
            cv::Canny(gray, canny, 30, 90);

            // Hough transform
            int angle_res = 700;
            int r_res = 350;
            const int w = canny.size().width, h = canny.size().height;
            cv::Mat hough(cv::Size(angle_res, 2 * r_res + 1), CV_32S, cv::Scalar::all(0));
            double rmax = sqrt(w * w + h * h);
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    if ((int) canny.at<uchar>(y, x) == 255) {
                        for (int a = 0; a < angle_res; a++) {
                            double phi = (double) a / angle_res * M_PI - 0.5 * M_PI;
                            double r = x * cos(phi) + y * sin(phi);
                            r = round(r * r_res / rmax + r_res);
                            hough.at<int>((int) r, a) += 1;
                        }
                    }
                }
            }

            cv::Mat hough_disp;
            cv::normalize(hough, hough_disp, 0, 255, cv::NORM_MINMAX, CV_8UC1);

            cv::imshow("gray", gray);
            cv::imshow("canny", canny);
            cv::imshow("hough", hough_disp);
            cv::waitKey();
        }
    }
}