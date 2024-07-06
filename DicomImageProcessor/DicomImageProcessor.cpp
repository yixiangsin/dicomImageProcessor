// DicomImageProcessor.cpp : Defines the entry point for the application.
//
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "DicomImageProcessor.h"

using namespace std;
using namespace cv;

cv::Rect DicomImageProcessor::indicatorRoiDetector(const cv::Mat& input)
{
        return cv::Rect();
}

cv::Rect DicomImageProcessor::edgeDetectionIndicatorRoiDetector(const cv::Mat& input)
{
        blur(input, input, Size(5, 5));
        Canny(input, input, 10, 30, 3);
        threshold(input, input, 10, 255, 1);
        Mat labels;
        connectedComponents(input, labels);

        return cv::Rect(-1, -1, -1, -1);
}

cv::Rect DicomImageProcessor::pixelThresholdDetectionIndicatorRoiDetector(const cv::Mat& input)
{
        return cv::Rect();
}

cv::Rect DicomImageProcessor::featuresDetectionIndicatorRoiDetector(const cv::Mat& input)
{
        return cv::Rect();
}



