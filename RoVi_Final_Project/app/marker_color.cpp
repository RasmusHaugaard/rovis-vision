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

    std::string imgPath = "../marker_color_hard/marker_color_hard_";
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

    cv::namedWindow("bgr", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("result", cv::WINDOW_AUTOSIZE);
    int j = 0;
    while (true) {
        std::cout << "loop: " << j++ << std::endl;
        for (int i = 0; i < 30; i++) {
            cv::Mat bgr = imgSequence[i];

            // hls
            cv::Mat hls;
            cv::cvtColor(bgr, hls, cv::COLOR_BGR2HLS);
            cv::Mat planes[3], h;
            cv::split(hls, planes);
            h = planes[0];

            // Canny edge
            cv::blur(h, h, cv::Size(5, 5));
            cv::Mat canny;
            cv::Canny(h, canny, 7, 31);
            auto cross = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
            cv::dilate(canny, canny, cross);

            // Contours
            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;
            findContours(canny, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
            cv::Mat drawing = cv::Mat::zeros(h.size(), CV_8UC3);

            std::vector<cv::Point> circlesFound;
            for (int c = 0; c < contours.size(); c++) {
                double area = cv::contourArea(contours[c]);
                if (area < 4000 || 13000 < area) continue;
                double perimeter = cv::arcLength(contours[c], true);
                double circleScore = 4 * M_PI * area / pow(perimeter, 2);
                if (circleScore < 0.8) continue;
                auto M = cv::moments(contours[c]);
                auto cx = M.m10 / M.m00, cy = M.m01 / M.m00;
                circlesFound.emplace_back(cx, cy);
                cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
                cv::drawContours(drawing, contours, c, color, 2, 8, hierarchy, 0, cv::Point());
            }

            // filter circles by distance, so only one match per circle is found
            for (int i1 = 0; i1 < circlesFound.size(); i1++) {
                auto p1 = circlesFound[i1];
                for (int i2 = i1 + 1; i2 < circlesFound.size(); i2++) {
                    auto p2 = circlesFound[i2];
                    auto d = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
                    if (d < 10) {
                        circlesFound.erase(circlesFound.begin() + i2);
                        i2--;
                    }
                }
            }
            if (circlesFound.size() == 4) {
                cv::Scalar white(255, 255, 255);
                cv::Scalar blue(255, 0, 0);
                cv::Scalar red(0, 0, 255);

                // distinguish between red and blue by hue at circle center
                cv::Point redCircle;
                for (auto &point : circlesFound) {

                    int circle_h = int(h.at<uchar>(point.y, point.x));
                    auto color = blue;
                    if (circle_h < 50 || 200 < circle_h) {
                        color = red;
                        redCircle = point;
                    }
                    cv::drawMarker(drawing, point, color);
                }

                // Find center point
                double cx = 0, cy = 0;
                for (auto &point : circlesFound) {
                    cx += point.x;
                    cy += point.y;
                }
                cv::Point center(
                        (int) round(cx / 4),
                        (int) round(cy / 4)
                );
                cv::drawMarker(drawing, center, white);

                std::vector<std::vector<cv::Point>> line(1);
                line[0] = {center, redCircle};
                cv::drawContours(drawing, line, 0, white);
            }

            cv::imshow("bgr", bgr);
            cv::imshow("canny", canny);
            cv::imshow("result", drawing);
            cv::waitKey(150);
        }
    }
}