#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "DicomImageProcessor.h"

cv::Rect topLeftIndicator(108, 90, 15, 15);
cv::Rect topRightIndicator(515, 90, 15, 15);
cv::Rect bottomLeftIndicator(108, 374, 15, 15);
cv::Rect bottomRightIndicator(515, 374, 15, 15);

class DicomImageProcessorTest : public ::DicomImageProcessor {

public:
        DicomImageProcessorTest() {
        }
        ~DicomImageProcessorTest() {
        }
        cv::Rect edgeDetectionIndicatorRoiDetector(const cv::Mat& input) override {
                return DicomImageProcessor::edgeDetectionIndicatorRoiDetector(input);
        }
        cv::Rect pixelThresholdDetectionIndicatorRoiDetector(const cv::Mat& input) override {
                return DicomImageProcessor::pixelThresholdDetectionIndicatorRoiDetector(input);
        }
};

// Example test that always passes
TEST(ExampleTest, AlwaysPass) {
        EXPECT_EQ(1, 1);
}

TEST(DicomImageProcessorTest, edgeDetectionIndicatorRoiDetector) {
        DicomImageProcessorTest dicomImageProcessor;

        // Check if the detected ROI is close to the top left indicator
        cv::Mat inputTopLeft = cv::imread("../../../../dicom/topLeft.jpeg", 0);
        cv::Rect intersectTopLeft = dicomImageProcessor.edgeDetectionIndicatorRoiDetector(inputTopLeft) &  topLeftIndicator;
        EXPECT_GT(intersectTopLeft.area(), topLeftIndicator.area()* 0.9);

        // Check if the detected ROI is close to the bottom right indicator
        cv::Mat inputBottomRight = cv::imread("../../../../dicom/bottomRight.png", 0);
        cv::Rect intersectBottomRight = dicomImageProcessor.edgeDetectionIndicatorRoiDetector(inputBottomRight) & bottomRightIndicator;
        EXPECT_GT(intersectBottomRight.area(), topLeftIndicator.area() * 0.9);
}

TEST(DicomImageProcessorTest, edgeDetectionIndicatorRoiDetectorEmptyImage) {
        DicomImageProcessorTest dicomImageProcessor;
        cv::Mat emptyInput;

        // Check if the function returns an negative ROI for an empty image
        cv::Rect roi = dicomImageProcessor.edgeDetectionIndicatorRoiDetector(emptyInput);
        EXPECT_EQ(roi.x, -1);
        EXPECT_EQ(roi.y, -1);
        EXPECT_EQ(roi.width, -1);
        EXPECT_EQ(roi.height,-1);
}

TEST(DicomImageProcessorTest, pixelThresholdDetectionIndicatorRoiDetector) {
        DicomImageProcessorTest dicomImageProcessor;

        // Check if the detected ROI is close to the top left indicator
        cv::Mat inputTopLeft = cv::imread("../../../../dicom/topLeft.jpeg", 0);
        cv::Rect intersectTopLeft = dicomImageProcessor.pixelThresholdDetectionIndicatorRoiDetector(inputTopLeft) & topLeftIndicator;
        EXPECT_GT(intersectTopLeft.area(), topLeftIndicator.area() * 0.9);

        // Check if the detected ROI is close to the bottom right indicator
        cv::Mat inputBottomRight = cv::imread("../../../../dicom/bottomRight.png", 0);
        cv::Rect intersectBottomRight = dicomImageProcessor.pixelThresholdDetectionIndicatorRoiDetector(inputBottomRight) & bottomRightIndicator;
        EXPECT_GT(intersectBottomRight.area(), topLeftIndicator.area() * 0.9);
}

TEST(DicomImageProcessorTest, pixelThresholdDetectionIndicatorRoiDetectorEmptyImage) {
        DicomImageProcessorTest dicomImageProcessor;
        cv::Mat emptyInput;

        // Check if the function returns an negative ROI for an empty image
        cv::Rect roi = dicomImageProcessor.pixelThresholdDetectionIndicatorRoiDetector(emptyInput);
        EXPECT_EQ(roi.x, -1);
        EXPECT_EQ(roi.y, -1);
        EXPECT_EQ(roi.width, -1);
        EXPECT_EQ(roi.height, -1);
}

// Main function
int main(int argc, char** argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}