#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <algorithm>
using namespace std;

cv::Mat histogram(const cv::Mat &img){
    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    cv::Mat hist;

    /// Compute the histograms:
    cv::calcHist( &img, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    cv::Mat histImage( hist_h, hist_w, CV_8UC1, cv::Scalar(0));

    /// Normalize the result to [ 0, histImage.rows ]
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line(
                histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                cv::Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                cv::Scalar(255), 2, 8, 0
        );
    }

    return histImage;
}

cv::Mat medianMinMax(const cv::Mat &img, const int kernelSize, uchar min = 0, uchar max = 255){
    cv::Mat out(img.rows, img.cols, CV_8UC1, cv::Scalar(0));

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
            out.at<uchar>(y, x) = vals[i / 2];
        }
    }

    return out;
}

cv::Mat intensityTransformation(const cv::Mat &img, const int intensity)
{
    uchar* imgPt = img.data;
    for (int r = 0; r < img.rows; r++)
        for (int c = 0; c < img.cols; c++)
            imgPt[(r*img.cols+c)] = imgPt[(r*img.cols+c)] + intensity;

    return img;
}

cv::Mat removeBorder(const cv::Mat &img, int border)
{
    int offset_x = border;
    int offset_y = border;
    cout << offset_x << " " << offset_y << endl;

    cv::Rect roi;
    roi.x = offset_x;
    roi.y = offset_y;

    roi.width = img.size().width - (offset_x*2);
    roi.height = img.size().height - (offset_y*2);

    /* Crop the original image to the defined ROI */

    cv::Mat crop(cv::Size(img.cols - border, img.rows - border), CV_8UC1, cv::Scalar(0));
    crop = img(roi);
    return crop;
}

/*
int main()
{
    // Load image file
    std::string filepath = "Images/ImagesForStudents/Image1.png";
    cv::Mat img1 = cv::imread(filepath, cv::IMREAD_GRAYSCALE);
    cv::Mat img;
    cv::resize(img1, img, cv::Size((img1.cols)/3, (img1.rows)/3), cv::INTER_CUBIC);

    auto hist_in = histogram(img);
    cv::imshow("Histogram Input", hist_in);

    auto median = medianMinMax(img, 5, 1, 255);
    auto InteTrans = intensityTransformation(median, 45);
    cv::medianBlur(InteTrans, InteTrans, 5);

    cv::imshow("Median", InteTrans);

    auto hist_median = histogram(InteTrans);
    cv::imshow("Histogram Output", hist_median);

    cv::waitKey(0);
    return 0;
}
 */
int main()
{
    // Load image file
    std::string filepath = "Images/ImagesForStudents/Image2.png";
    cv::Mat img1 = cv::imread(filepath, cv::IMREAD_GRAYSCALE);
    cv::Mat img;
    cv::resize(img1, img, cv::Size((img1.cols)/3, (img1.rows)/3), cv::INTER_CUBIC);

    cv::imshow("image", img);

    auto hist_in = histogram(img);
    cv::imshow("Histogram Input", hist_in);

    auto median = medianMinMax(img, 7, 5, 250);
    cv::imshow("Median", median);

    auto cropped = removeBorder(median, 5);

    auto hist_median = histogram(cropped);
    cv::imshow("Histogram Output", hist_median);

    cv::Mat eqHist;
    cv::equalizeHist(cropped, eqHist);
    cv::Mat new_median;
    cv::medianBlur(eqHist, new_median, 5);
    cv::imshow("after eq hist", new_median);

    auto hist_eq = histogram(new_median);
    cv::imshow("hist after eq hist", hist_eq);




    cv::waitKey(0);
    return 0;
}