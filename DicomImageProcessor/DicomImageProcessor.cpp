// DicomImageProcessor.cpp : Defines the entry point for the application.
//
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>
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

bool DicomImageProcessor::dicomImageRotateCorrection(cv::Mat& input, const IndicatorDetectionMethod& method)
{
        if (input.empty())
        {
                return false;
        }
        Mat inputCopy = input.clone();
        cv::Rect roi = indicatorRoiDetector(inputCopy, method);

        if ((roi & topLeftIndicator.position).area() > topLeftIndicator.size)
        {
                return true;
        }
        else if ((roi & bottomRightIndicator.position).area() > bottomRightIndicator.size)
        {
                cv::rotate(input, input, ROTATE_180);
                return true;
        }
        return false;

}

const Indicator& DicomImageProcessor::getBottomRightIndicator() const
{
        return bottomRightIndicator;
}

const cv::Mat DicomImageProcessor::generateHistogram(const cv::Mat& input)
{
        if (input.empty())
        {
                return Mat();
        }
        //ignore the pixel value 0 and 255
        const size_t maxHistValue = 254;
        const size_t minHistValue = 1;
        // dilate the image to remove line and number
        dilate(input, input, Mat(), Point(-1, -1), 2);

        Mat hist;
        int histSize = maxHistValue;
        float range[] = { minHistValue, maxHistValue };
        const float* histRange = { range };
        calcHist(&input, 1, 0, Mat(), hist, 1, &histSize, &histRange);

        // Normalize the result to [0, histImage.rows]
        cv::normalize(hist, hist, 0, maxHistValue, cv::NORM_MINMAX);

        int hist_w = 512; int hist_h = 400;
        int bin_w = cvRound((double)hist_w / histSize);
        Mat histImage(hist_h, hist_w + 50, CV_8UC1, Scalar(255));

        int binWidth = cvRound((double)(histImage.cols - 50) / histSize);
        // Draw the histogram
        for (int i = 1; i < histSize; i++) {
                line(histImage,
                        Point(50 + binWidth * (i - 1), histImage.rows - 50 - cvRound(hist.at<float>(i - 1))),
                        Point(50 + binWidth * (i), histImage.rows - 50 - cvRound(hist.at<float>(i))),
                        Scalar(0), 2, 8, 0);
        }
        // Draw the x and y axes
        line(histImage, Point(50, 0), Point(50, histImage.rows - 50), Scalar(10), 2); // Y-axis
        line(histImage, Point(50, histImage.rows - 50), Point(histImage.cols, histImage.rows - 50), Scalar(50), 2); // X-axis

        // Annotate the histogram with numbers on x-axis
        for (int i = 0; i < histSize; i += 32) { // Annotate every 32 bins
                int x = 50 + binWidth * i;
                putText(histImage, to_string(i), Point(x, histImage.rows - 30), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(50), 1, LINE_AA);
        }

        // Annotate the histogram with numbers on y-axis
        int maxVal = 255;
        for (int i = 0; i <= maxVal; i += 50) {
                int y = histImage.rows - 50 - cvRound((float)i * histImage.rows / (float)maxVal);
                putText(histImage, to_string(i), Point(10, y), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(50), 1, LINE_AA);
        }

        // Display the histogram
        return histImage;
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
        // dilate the image to remove line and number
        dilate(input, input, Mat(), Point(-1, -1), 2);
        Mat inputCopy = input.clone();
        blur(input, input, Size(5, 5));
        Canny(input, input, 10, 30, 3);
        dilate(input, input, Mat(), Point(-1, -1), 2);
        std::vector<Rect> rois = findIndicatorRoi(input, 300, 400, 0.9f, 1.1f, 0.5f);
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
        // dilate the image to remove line and number
        dilate(input, input, Mat(), Point(-1, -1), 2);
        Mat inputCopy = input.clone();
        Mat inputInv = 255 - input;
        threshold(inputInv, inputInv, 10, 255, 1);
        dilate(inputInv, inputInv, Mat(), Point(-1, -1), 2);

        std::vector<Rect> rois = findIndicatorRoi(inputInv, 250, 400, 0.8f, 1.2f, 0.7f);
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

std::vector<cv::Rect> DicomImageProcessor::findIndicatorRoi(const cv::Mat& input,
        const size_t& rectAreaMin,
        const size_t& rectAreaMax,
        const float& widthHeightRatioMin,
        const float& widthHeightRatioMax,
        const float& areaDensityRatioThres)
{
        std::vector<cv::Rect> rois;
        Mat labelImage(input.size(), CV_32S);
        Mat stats, centroids;
        int nLabels = connectedComponentsWithStats(input, labelImage, stats, centroids, 8, CV_32S);
        for (int label = 1; label < nLabels; ++label) { //label  0 is the background
                size_t area = stats.at<int>(label, CC_STAT_WIDTH) * stats.at<int>(label, CC_STAT_HEIGHT);
                float areaDensity = float(stats.at<int>(label, CC_STAT_AREA)) / float(area);
                double widthHeightRatio = double(stats.at<int>(label, CC_STAT_WIDTH)) / double(stats.at<int>(label, CC_STAT_HEIGHT));
                if (stats.at<int>(label, CC_STAT_AREA) > rectAreaMin &&
                        stats.at<int>(label, CC_STAT_AREA) < rectAreaMax &&
                        widthHeightRatio > widthHeightRatioMin && widthHeightRatio < widthHeightRatioMax){
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
                        size_t area = area = cv::contourArea(contour);
                        float areaDensity = float(area) / float(roi.area());
                        if (roi.area() > rectAreaMin &&
                                roi.area() < rectAreaMax &&
                                widthHeightRatio > widthHeightRatioMin && widthHeightRatio < widthHeightRatioMax
                                && areaDensity > areaDensityRatioThres)
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

void DicomImageProcessor::dicomImagePreprocessing(cv::Mat& input)
{
}

Indicator::Indicator(const cv::Rect& position, const size_t size) : position(position), size(size)
{
}

