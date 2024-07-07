// DicomImageProcessor.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

class DicomImageProcessor {
public:
        enum IndicatorDetectionMethod {
                EDGE_DETECTION,
                PIXEL_THRESHOLD,
                FEATURES_DETECTION
        };

        cv::Rect indicatorRoiDetector(const cv::Mat& input, const IndicatorDetectionMethod& method);

protected:
        virtual cv::Rect edgeDetectionIndicatorRoiDetector(const cv::Mat& input);
        virtual cv::Rect pixelThresholdDetectionIndicatorRoiDetector(const cv::Mat& input);
        virtual cv::Rect featuresDetectionIndicatorRoiDetector(const cv::Mat& input);

private:
        std::vector<cv::Rect> findIndicatorRoi(const cv::Mat& input);
        void drawRectangle(cv::Mat& input, const cv::Rect& roi);


};

// TODO: Reference additional headers your program requires here.
