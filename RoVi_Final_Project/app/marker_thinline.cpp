#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

double cross(cv::Point v1, cv::Point v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}

bool getIntersectionPoint(cv::Point &a1, cv::Point &a2, cv::Point &b1, cv::Point &b2, cv::Point & intPnt)
{
    cv::Point r(a2-a1);
    cv::Point s(b2-b1);

    if(cross(r,s) == 0)
    {
        std::cout << "No intersection" << std::endl;
        return false;
    }

    double t = cross(b1-a1,s)/cross(r,s);
    intPnt = a1 + t*r;

    return true;
}


int main() {
    cv::RNG rng(12345);

    std::string imgPath = "../marker_thinline/marker_thinline_";
    std::string fileType = ".png";
    std::vector<cv::Mat> imgSequence, imgGray, imgInv, imgCanny, imgLines;
    cv::Mat imgTemp;

    for (int i = 1; i < 31; i++)
    {
        if (i < 10)
            imgTemp = cv::imread(imgPath + "0" + std::to_string(i) + fileType);
        else
            imgTemp = cv::imread(imgPath + std::to_string(i) + fileType);
        imgSequence.push_back(imgTemp);
        imgGray.emplace_back();
        imgInv.emplace_back();
        imgLines.emplace_back(cv::Mat(cv::Size(imgTemp.cols, imgTemp.rows), CV_8UC3, cv::Scalar(0,0,0)));
        imgCanny.emplace_back();
    }

    cv::namedWindow("img", cv::WINDOW_AUTOSIZE);
    //cv::namedWindow("imgInv", cv::WINDOW_AUTOSIZE);
    //cv::namedWindow("imgCanny", cv::WINDOW_AUTOSIZE);
    //cv::namedWindow("imgLines", cv::WINDOW_AUTOSIZE);

    //cv::imshow("img", thr1);
    //cv::waitKey(0);


    while (true)
    {
        for(int i = 0; i < 30; i++)
        {
            cv::Mat hls;
            cv::cvtColor(imgSequence[i], hls, cv::COLOR_BGR2HLS);
            cv::Mat planes[3], l_mat, thr1;
            cv::split(hls, planes);
            l_mat = planes[1];

            cv::threshold(l_mat, thr1, 75, 200, cv::THRESH_BINARY);

            cv::bitwise_not(thr1, imgInv[i]);
            cv::blur(imgInv[i], imgInv[i], cv::Size(5,5));
            cv::Canny(imgInv[i], imgCanny[i], 7, 31);

            int crossSize = 11;
            auto cross = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(crossSize, crossSize));
            cv::dilate(imgCanny[i], imgCanny[i], cross);
            cross = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(crossSize, crossSize));
            cv::erode(imgCanny[i], imgCanny[i], cross);


            //Find lines and draw them to a new image:
            std::vector<cv::Vec4i> lines;
            HoughLinesP(imgCanny[i], lines, 1, CV_PI/270, 8, 220, 18);
            cv::Point marker;
            std::vector<cv::Point> markers;
            for (int l = 0; l < lines.size() - 1; l++)
            {
                cv::Point a1(lines[l][0], lines[l][1]);
                cv::Point a2(lines[l][2], lines[l][3]);
                for (int j = 1+i; j < lines.size(); j++)
                {
                    cv::Point b1(lines[j][0], lines[j][1]);
                    cv::Point b2(lines[j][2], lines[j][3]);

                    if(getIntersectionPoint(a1, a2, b1, b2, marker))
                    {
                        cv::circle(imgSequence[i], marker, 8, cv::Scalar(0,255,0), -1);
                        markers.push_back(marker);
                    }

                }
            }
            for (int m = 0; m < markers.size(); m++)
            {
                int m_dist = markers[m].x * markers[m].y;
                for (int mx = m+1; mx < markers.size(); mx++)
                {
                    int dist_counter = 0;
                    int mx_dist = markers[mx].x * markers[mx].y;

                    if (m_dist > mx_dist - 2 && m_dist < mx_dist + 2)
                        dist_counter++;

                    if (dist_counter < 3)
                    {
                        markers.erase(markers.begin() + m);
                        m--;
                    }
                }
            }

            std::cout << "Number of lines: " << lines.size() << std::endl;
            for(auto &l : lines)
                cv::line( imgSequence[i], cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, CV_AA);

            cv::imshow("img", imgSequence[i]);
            //cv::imshow("imgInv", imgInv[i]);
            //cv::imshow("imgCanny", imgCanny[i]);
            //cv::imshow("imgLines", imgLines[i]);
            cv::waitKey(0);
        }
    }


}