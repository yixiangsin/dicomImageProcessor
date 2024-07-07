// DicomImageProcessor.cpp : Defines the entry point for the application.
//
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "DicomImageProcessor.h"

using namespace std;
using namespace cv;

DicomImageProcessor::DicomImageProcessor() : topLeftIndicator(cv::Rect(108, 90, 15, 15), 195),   bottomRightIndicator(cv::Rect(515, 374, 15, 15), 195)
{
}

const Indicator& DicomImageProcessor::getTopLeftIndicator() const
{
        return topLeftIndicator;
}

bool DicomImageProcessor::dicomImageRotateCorrection(cv::Mat& input)
{
        if (input.empty())
        {
                return false;
        }
}

const Indicator& DicomImageProcessor::getBottomRightIndicator() const
{
        return bottomRightIndicator;
}

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
        if (input.empty())
        {
                return cv::Rect(-1, -1, -1, -1);
        }
        Mat inputCopy = input.clone();
        Mat inputInv = 255 - input;
        threshold(inputInv, inputInv, 10, 255, 1);
        dilate(inputInv, inputInv, Mat(), Point(-1, -1), 2);

        std::vector<Rect> rois = findIndicatorRoi(inputInv);
        for (auto roi : rois)
        {
                drawRectangle(inputCopy, roi);
        }

        //imshow("result", inputCopy);
        //imshow("img processed", inputInv);

        if (rois.size() == 1)
        {
                return rois[0];
        }

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
        vector<vector<Point>> contours;
        if(rois.size() == 0)
        {
                findContours(input, contours, RETR_LIST, CHAIN_APPROX_NONE);
                for (auto contour : contours)
                {
                        Rect roi = boundingRect(contour);
                        double widthHeightRatio = double(roi.width) / double(roi.height);
                        if (roi.area() > 250 &&
                                roi.area() < 400 &&
                                widthHeightRatio > 0.9 && widthHeightRatio < 1.1)
                        {
                                rois.push_back(roi);
                        }
                }
        }
        contours.clear();
        return rois;
}

void DicomImageProcessor::drawRectangle(cv::Mat& input, const cv::Rect& roi)
{
        cv::rectangle(input, roi, cv::Scalar(255, 0, 0), 2);
}

Indicator::Indicator(const cv::Rect& position, const size_t size) : position(position), size(size)
{
}

