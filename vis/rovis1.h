#ifndef ROVIS_VISION_ROVIS1_H
#define ROVIS_VISION_ROVIS1_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "vis.h"

namespace rovis1 {
    const cv::Point ev1Center(1228, 1578);
    const cv::Point ev2Center(1328, 578);
    const cv::Rect ev1Rect(1100, 1450, 256, 256);
    const cv::Rect ev2Rect(1200, 450, 256, 256);

    void imageAnalysis(const cv::String &folder, const cv::String &title, const cv::Mat &src) {
        const int height = 256 * 2;
        const double scale = double(height) / src.rows;
        const int evSize = 256;

        // evaluation regions
        auto ev1 = src(rovis1::ev1Rect);
        auto ev2 = src(rovis1::ev2Rect);
        auto ev1hist = vis::histImage(vis::calcHist(ev1));
        auto ev2hist = vis::histImage(vis::calcHist(ev2));

        // full image
        auto hist = vis::histImage(vis::calcHist(src));
        const int width = int(round(scale * src.cols));
        cv::Mat fullImg(height, width, CV_8UC1);
        cv::resize(src, fullImg, fullImg.size(), 0, 0, CV_INTER_AREA);
        for (const auto &r : {ev1Rect, ev2Rect}) {
            int x = int(round(r.x * scale)), y = int(round(r.y * scale));
            int w = int(round(r.width * scale)), h = int(round(r.height * scale));
            cv::rectangle(fullImg, cv::Rect(x, y, w, h), 0);
        }

        // stick together image
        cv::Mat dst(height, width + 2 * height, CV_8UC1, cv::Scalar(0));
        fullImg.copyTo(cv::Mat(dst, cv::Rect(0, 0, width, height)));
        cv::resize(hist, hist, cv::Size(height, height), 0, 0, CV_INTER_NN);
        hist.copyTo(cv::Mat(dst, cv::Rect(width, 0, height, height)));
        ev1.copyTo(cv::Mat(dst, cv::Rect(width + height, 0, evSize, evSize)));
        ev2.copyTo(cv::Mat(dst, cv::Rect(width + height, evSize, evSize, evSize)));
        ev1hist.copyTo(cv::Mat(dst, cv::Rect(width + height + evSize, 0, evSize, evSize)));
        ev2hist.copyTo(cv::Mat(dst, cv::Rect(width + height + evSize, evSize, evSize, evSize)));

        cv::imwrite("../results/" + folder + "/" + title + ".png", dst);
    }

    double calcEv1Var(const cv::Mat &src) {
        auto ev1 = vis::extractKernel(src, ev1Center, 11);
        return vis::meanvar(&*ev1.begin(), &*ev1.end()).second;
    }

}

#endif //ROVIS_VISION_ROVIS1_H
