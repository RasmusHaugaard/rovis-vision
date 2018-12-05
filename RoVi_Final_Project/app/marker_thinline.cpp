#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <cmath>
#include <math.h>


double findAngle(cv::Point &p11, cv::Point &p12, cv::Point &p21, cv::Point &p22)
{
    /*
    double m1 = double(abs(p12.y - p11.y)) / double(abs(p12.x - p11.x));   // tan(A)
    double A = atan(m1) * 180 / M_PI;

    double m2 = double(abs(p22.y - p21.y)) / double(abs(p22.x - p21.x));  // tan(B)
    double B = atan(m2) * 180 / M_PI;

    std::cout << atan2(p12.y - p11.y, p12.x - p11.x) << std::endl;
    std::cout << atan2(p22.y - p21.y, p22.x - p21.x) << std::endl;

    std::cout << "A: " << A << "  B: " << B << "   B - A = " << B+A << std::endl;
*/
    double A = atan2(p12.y - p11.y, p12.x - p11.x);
    double B = atan2(p22.y - p21.y, p22.x - p21.x);

    return (fabsf(A-B))*180/M_PI;
}

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
        //std::cout << "No intersection" << std::endl;
        return false;
    }

    double t = cross(b1-a1,s)/cross(r,s);
    intPnt = a1 + t*r;

    std::vector<int> x_list;
    x_list.push_back(a1.x); x_list.push_back(a2.x);
    std::vector<int> y_list;
    y_list.push_back(a1.y); y_list.push_back(a2.y);

    std::sort(x_list.begin(), x_list.end());
    std::sort(y_list.begin(), y_list.end());

    if ( intPnt.x < x_list[0] || intPnt.x > x_list[1] )
        return false;
    if ( intPnt.y < y_list[0] || intPnt.y > y_list[1] )
        return false;

    x_list.clear(); y_list.clear();
    x_list.push_back(b1.x); x_list.push_back(b2.x); y_list.push_back(b1.y); y_list.push_back(b2.y);

    std::sort(x_list.begin(), x_list.end());
    std::sort(y_list.begin(), y_list.end());

    if ( intPnt.x < x_list[0] || intPnt.x > x_list[1] )
        return false;
    if ( intPnt.y < y_list[0] || intPnt.y > y_list[1] )
        return false;

    double tmpAng = findAngle(a1, a2, b1, b2);
    //std::cout << "temp ang: " << tmpAng << std::endl;

    if (tmpAng < 80 || tmpAng > 100)
        return false;
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
        for(int i = 18; i < 30; i++)
        {
            std::cout << "imgae nr: " << i << std::endl;

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
            std::vector<cv::Vec4i> lines, lines_f;
            HoughLinesP(imgCanny[i], lines, 1, CV_PI/270, 8, 220, 18);


            //std::cout << lines.size() << std::endl;
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