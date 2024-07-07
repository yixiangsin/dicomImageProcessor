// DicomImageProcessor.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

struct Indicator {

        Indicator(const cv::Rect& position, const size_t size);
        const cv::Rect position;
        const size_t size;
};

class DicomImageProcessor {
public:
        enum IndicatorDetectionMethod {
                EDGE_DETECTION,
                PIXEL_THRESHOLD,
                FEATURES_DETECTION
        };

        cv::Rect indicatorRoiDetector(const cv::Mat& input, const IndicatorDetectionMethod& method);
        const Indicator& getTopLeftIndicator() const;
        const Indicator& getBottomRightIndicator() const;
        bool dicomImageRotateCorrection(cv::Mat& input, const IndicatorDetectionMethod& method = IndicatorDetectionMethod::EDGE_DETECTION);
        DicomImageProcessor();

protected:
        virtual cv::Rect edgeDetectionIndicatorRoiDetector(const cv::Mat& input);
        virtual cv::Rect pixelThresholdDetectionIndicatorRoiDetector(const cv::Mat& input);
        virtual cv::Rect featuresDetectionIndicatorRoiDetector(const cv::Mat& input);

private:
        const Indicator topLeftIndicator, bottomRightIndicator;
        std::vector<cv::Rect> findIndicatorRoi(const cv::Mat& input);
        void drawRectangle(cv::Mat& input, const cv::Rect& roi);


};

// TODO: Reference additional headers your program requires here.
