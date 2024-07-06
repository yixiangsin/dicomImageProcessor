// DicomImageProcessor.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

class DicomImageProcessor {
public:

        cv::Rect indicatorRoiDetector(const cv::Mat& input);

protected:
        virtual cv::Rect edgeDetectionIndicatorRoiDetector(const cv::Mat& input);
        virtual cv::Rect pixelThresholdDetectionIndicatorRoiDetector(const cv::Mat& input);
        virtual cv::Rect featuresDetectionIndicatorRoiDetector(const cv::Mat& input);

private:


};

// TODO: Reference additional headers your program requires here.
