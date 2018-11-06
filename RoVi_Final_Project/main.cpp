#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;

int main()
{
    cv::RNG rng(12345);

    std::string imgPath = "../marker_color/marker_color_";
    std::string fileType = ".png";
    std::vector<cv::Mat> imgSequence1, imgSequence1gray, imgSequence1canny;
    cv::Mat imgTemp;

    for (int i = 1; i < 31; i++)
    {
        if(i < 10)
            imgTemp = cv::imread(imgPath + "0" + std::to_string(i) + fileType);
        else
            imgTemp = cv::imread(imgPath + std::to_string(i) + fileType);
        imgSequence1.push_back(imgTemp);
        imgSequence1gray.push_back(cv::Mat());
        imgSequence1canny.push_back(cv::Mat());
    }

    cv::namedWindow("Sequenze1", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Sequenze1gray", cv::WINDOW_AUTOSIZE);
    cv::namedWindow( "Contours", cv::WINDOW_AUTOSIZE );
    int j = 0;
    while (true)
    {
        cout << j++ << endl;
        for(int i = 0; i < 30; i++)
        {
            cv::cvtColor( imgSequence1[i], imgSequence1gray[i], cv::COLOR_BGR2GRAY );
            cv::blur(imgSequence1gray[i], imgSequence1gray[i], cv::Size(3,3));
            cv::Canny(imgSequence1gray[i], imgSequence1canny[i], 8, 35);

            std::vector<vector<cv::Point> > contours;
            std::vector<cv::Vec4i> hierarchy;
            findContours( imgSequence1canny[i], contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
            cv::Mat drawing = cv::Mat::zeros( imgSequence1canny[i].size(), CV_8UC3 );
            for( int c = 0; c < contours.size(); c++ )
            {
                cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                drawContours( drawing, contours, c, color, 2, 8, hierarchy, 0, cv::Point() );
            }

            cv::imshow("Sequenze1", imgSequence1gray[i]);
            cv::imshow("Sequenze1gray", imgSequence1canny[i]);
            cv::imshow( "Contours", drawing );
            cv::waitKey(150);


        }
    }


    return 0;
}