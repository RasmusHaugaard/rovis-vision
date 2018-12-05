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

double get_dist(cv::Point &p1, cv::Point &p2)
{
    sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

void removeClusters(std::vector<cv::Point> &mks)
{
    std::vector<cv::Point> tmpVec;
    std::vector<cv::Point> newVec;
    for (int i = 0; i < mks.size(); i++)
    {
        tmpVec.clear();
        tmpVec.push_back(mks[i]);
        for (int j = 0; j < mks.size(); j++)
        {
            if (i == j) continue;
            if (get_dist(mks[i], mks[j]) < 10)
            {
                tmpVec.push_back(mks[j]);
                mks.erase(mks.begin()+j);
                j--;
            }
        }
        if (tmpVec.size() < 4) continue;
        int tmpSumX = 0;
        int tmpSumY = 0;
        for (auto &v : tmpVec)
        {
            tmpSumX += v.x;
            tmpSumY += v.y;
        }
        newVec.emplace_back(tmpSumX/tmpVec.size(), tmpSumY/tmpVec.size());
    }
    mks.clear();
    mks = newVec;

}

double findAngle(cv::Point &p11, cv::Point &p12, cv::Point &p21, cv::Point &p22)
{
    double A = atan2(p12.y - p11.y, p12.x - p11.x);
    double B = atan2(p22.y - p21.y, p22.x - p21.x);

    return (fabsf(A-B))*180/M_PI;
}

double cross(cv::Point v1, cv::Point v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}

bool getIntersectionPoint(cv::Point &a1, cv::Point &a2, cv::Point &b1, cv::Point &b2, cv::Point & intPnt, cv::Mat &img)
{
    cv::Point r(a2-a1);
    cv::Point s(b2-b1);

    double tmpAng = findAngle(a1, a2, b1, b2);
    //std::cout << "temp ang: " << tmpAng << std::endl;

    if(cross(r,s) == 0)
    {
        //std::cout << "No intersection" << std::endl;
        return false;
    }

    double t = cross(b1-a1,s)/cross(r,s);
    intPnt = a1 + t*r;
    //std::cout << "int pnt" << intPnt << std::endl;

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

    if ( intPnt.x < x_list[0] - 25 || intPnt.x > x_list[1] + 25 )
        return false;

    if ( intPnt.y < y_list[0] - 25 || intPnt.y > y_list[1] + 25 )
        return false;

    if (tmpAng < 80 || tmpAng > 100)
        return false;

    return true;
}


int main() {
    cv::RNG rng(12345);

    std::string imgPath = "../marker_thinline/marker_thinline_";
    std::string fileType = ".png";
    std::vector<cv::Mat> imgSequence, imgGray, imgInv, imgCanny, imgLines, l_mat, thr1;
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
        l_mat.emplace_back();
        thr1.emplace_back();
    }

    cv::namedWindow("img", cv::WINDOW_AUTOSIZE);
    //cv::namedWindow("imgInv", cv::WINDOW_AUTOSIZE);
    //cv::namedWindow("imgCanny", cv::WINDOW_AUTOSIZE);
    //cv::namedWindow("imgLines", cv::WINDOW_AUTOSIZE);

    while (true)
    {
        for(int i = 0; i < 30; i++)
        {
            std::cout << std::endl << "imgae nr: " << i << std::endl;
            imgCanny[i].release();

            //Make an hls of the image an take out the 'l'
            cv::Mat hls;
            hls.release();
            cv::cvtColor(imgSequence[i], hls, cv::COLOR_BGR2HLS);
            cv::Mat planes[3];
            planes->release();
            cv::split(hls, planes);
            l_mat[i] = planes[1];

            //Threshold the l_mat to remove background noise
            cv::threshold(l_mat[i], thr1[i], 75, 200, cv::THRESH_BINARY);

            //invert and blur the image to get a better canny result
            cv::bitwise_not(thr1[i], imgInv[i]);
            cv::blur(imgInv[i], imgInv[i], cv::Size(5,5));
            cv::Canny(imgInv[i], imgCanny[i], 7, 31);

            //dialte and erode the iamge in order to do closing, to close the lines from canny
            int crossSize = 11;
            auto cross = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(crossSize, crossSize));
            cv::dilate(imgCanny[i], imgCanny[i], cross);
            cross = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(crossSize, crossSize));
            cv::erode(imgCanny[i], imgCanny[i], cross);


            //Find lines using hough transform:
            std::vector<cv::Vec4i> lines, lines_f;
            lines.clear(); lines_f.clear();
            HoughLinesP(imgCanny[i], lines, 1, CV_PI/270, 8, 220, 18);
            std::cout << "Nr. of Lines: " << lines.size() << std::endl;


            //find intersection between lines and draw them as points
            cv::Point marker;
            std::vector<cv::Point> markers;
            for (int l = 0; l < lines.size() - 1; l++)
            {
                cv::Point a1(lines[l][0], lines[l][1]);
                cv::Point a2(lines[l][2], lines[l][3]);
                for (int j = 1+l; j < lines.size(); j++)
                {
                    cv::Point b1(lines[j][0], lines[j][1]);
                    cv::Point b2(lines[j][2], lines[j][3]);

                    if(getIntersectionPoint(a1, a2, b1, b2, marker, imgSequence[i]))
                    {
                        /*
                        //cv::circle(imgSequence[i], marker, 8, cv::Scalar(0,255,0), -1);
                        auto ptr = &(imgSequence[i].at<uchar>(marker.y, marker.x));
                        uchar b = *ptr;
                        uchar g = *(ptr+1);
                        uchar r = *(ptr+2);
                        std::cout << (int)b << " , " << (int)g << " , " << (int)r << std::endl;
                        */
                        markers.push_back(marker);
                    }
                }
            }
            std::cout << "Nr. of Markers before: " << markers.size() << std::endl;
            removeClusters(markers);
            std::cout << "Nr. of Markers after: " << markers.size() << std::endl;

            for (auto &m : markers)
                cv::circle(imgSequence[i], m, 4, cv::Scalar(0,255,0), -1);


            //Draw the lines:
            //for(auto &l : lines)
                //cv::line( imgSequence[i], cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, CV_AA);

            //show images
            cv::imshow("img", imgSequence[i]);
            //cv::imshow("imgInv", imgInv[i]);
            //cv::imshow("imgCanny", imgCanny[i]);
            //cv::imshow("imgLines", imgLines[i]);
            cv::waitKey(0);
        }
    }


}