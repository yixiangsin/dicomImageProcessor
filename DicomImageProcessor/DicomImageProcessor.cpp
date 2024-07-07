// DicomImageProcessor.cpp : Defines the entry point for the application.
//
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "DicomImageProcessor.h"

using namespace std;
using namespace cv;

cv::Rect DicomImageProcessor::indicatorRoiDetector(const cv::Mat& input, const IndicatorDetectionMethod& method)
{
        switch (method)
        {
                case EDGE_DETECTION:
                        return edgeDetectionIndicatorRoiDetector(input);
                case PIXEL_THRESHOLD:
                        return pixelThresholdDetectionIndicatorRoiDetector(input);
                case FEATURES_DETECTION:
                        return featuresDetectionIndicatorRoiDetector(input);
        };
        return cv::Rect();
}

cv::Rect DicomImageProcessor::edgeDetectionIndicatorRoiDetector(const cv::Mat& input)
{
        if (input.empty())
        {
                return cv::Rect(-1, -1, -1, -1);
        }
        Mat inputCopy = input.clone();
        blur(input, input, Size(5, 5));
        Canny(input, input, 10, 30, 3);
        dilate(input, input, Mat(), Point(-1, -1), 2);
        std::vector<Rect> rois = findIndicatorRoi(input);
        for (auto roi : rois)
        {
                drawRectangle(inputCopy, roi);
        }
        //imshow("img processed", input);
        //imshow("result", inputCopy);

        if (rois.size() == 1)
        {
                return rois[0];
        }
        return cv::Rect(-1, -1, -1, -1);
}

cv::Rect DicomImageProcessor::pixelThresholdDetectionIndicatorRoiDetector(const cv::Mat& input)
{
        Mat inputInv = 255 - input;
        threshold(inputInv, inputInv, 10, 255, 1);
        Mat labels;
        connectedComponents(input, labels);
        imshow("result", inputInv);


        return cv::Rect();
}

cv::Rect DicomImageProcessor::featuresDetectionIndicatorRoiDetector(const cv::Mat& input)
{
        return cv::Rect();
}

std::vector<cv::Rect> DicomImageProcessor::findIndicatorRoi(const cv::Mat& input)
{
        std::vector<cv::Rect> rois;
        Mat labelImage(input.size(), CV_32S);
        Mat stats, centroids;
        int nLabels = connectedComponentsWithStats(input, labelImage, stats, centroids, 8, CV_32S);
        for (int label = 1; label < nLabels; ++label) { //label  0 is the background
                double widthHeightRatio = double(stats.at<int>(label, CC_STAT_WIDTH)) / double(stats.at<int>(label, CC_STAT_HEIGHT));
                if (stats.at<int>(label, CC_STAT_AREA) > 300 &&
                        stats.at<int>(label, CC_STAT_AREA) < 400 &&
                        widthHeightRatio > 0.9 && widthHeightRatio < 1.1){
                        rois.push_back(
                                cv::Rect(stats.at<int>(label, CC_STAT_LEFT),
                                        stats.at<int>(label, CC_STAT_TOP),
                                        stats.at<int>(label, CC_STAT_WIDTH),
                                        stats.at<int>(label, CC_STAT_HEIGHT)));
                }
        }
        return rois;
}

void DicomImageProcessor::drawRectangle(cv::Mat& input, const cv::Rect& roi)
{
        cv::rectangle(input, roi, cv::Scalar(255, 0, 0), 2);
}



