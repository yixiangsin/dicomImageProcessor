#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "DicomImageProcessor.h"

using namespace std;
using namespace cv;

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
        std::vector<cv::Rect> findIndicatorRoi(const cv::Mat& input, const size_t& rectAreaMin,
                const size_t& rectAreaMax,
                const float& widtHeightRatioMin,
                const float& widthHeightRatioMax,
                const float& areaDensityRatio)
        {
                return DicomImageProcessor::findIndicatorRoi(input,
                        rectAreaMin,
                        rectAreaMax,
                        widtHeightRatioMin,
                        widthHeightRatioMax,
                        areaDensityRatio);
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
        cv::Rect intersectTopLeft = dicomImageProcessor.edgeDetectionIndicatorRoiDetector(inputTopLeft) & dicomImageProcessor.getTopLeftIndicator().position;
        EXPECT_GT(intersectTopLeft.area(), dicomImageProcessor.getTopLeftIndicator().size);

        // Check if the detected ROI is close to the bottom right indicator
        cv::Mat inputBottomRight = cv::imread("../../../../dicom/bottomRight.png", 0);
        cv::Rect intersectBottomRight = dicomImageProcessor.edgeDetectionIndicatorRoiDetector(inputBottomRight) & dicomImageProcessor.getBottomRightIndicator().position;
        EXPECT_GT(intersectBottomRight.area(), dicomImageProcessor.getBottomRightIndicator().size);
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
        cv::Rect intersectTopLeft = dicomImageProcessor.pixelThresholdDetectionIndicatorRoiDetector(inputTopLeft) & dicomImageProcessor.getTopLeftIndicator().position;
        EXPECT_GT(intersectTopLeft.area(), dicomImageProcessor.getTopLeftIndicator().size);

        // Check if the detected ROI is close to the bottom right indicator
        cv::Mat inputBottomRight = cv::imread("../../../../dicom/bottomRight.png", 0);
        cv::Rect intersectBottomRight = dicomImageProcessor.pixelThresholdDetectionIndicatorRoiDetector(inputBottomRight) & dicomImageProcessor.getBottomRightIndicator().position;
        EXPECT_GT(intersectBottomRight.area(), dicomImageProcessor.getBottomRightIndicator().size);
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

TEST(DicomImageProcessorTest, dicomImageRotateCorrectionEmptyImageTest) {
        DicomImageProcessorTest dicomImageProcessor;
        cv::Mat emptyInput;

        // Check if the function returns false for an empty image
        EXPECT_FALSE(dicomImageProcessor.dicomImageRotateCorrection(emptyInput));
}

TEST(DicomImageProcessorTest, dicomImageRotateCorrectionTest) {
        DicomImageProcessorTest dicomImageProcessor;
        cv::Mat inputNoRotate = cv::imread("../../../../dicom/topLeft.jpeg", 0);
        cv::Mat inputNoRotateCopy = inputNoRotate.clone();
        dicomImageProcessor.dicomImageRotateCorrection(inputNoRotate);
        EXPECT_EQ(inputNoRotate.size(), inputNoRotateCopy.size());
        EXPECT_EQ(inputNoRotate.type(), inputNoRotateCopy.type());

        cv::Mat diffNoRotate;
        cv::bitwise_xor(inputNoRotate, inputNoRotateCopy, diffNoRotate);
        EXPECT_EQ(cv::countNonZero(diffNoRotate), 0);


        cv::Mat inputRotate = cv::imread("../../../../dicom/bottomRight.png", 0);
        cv::Mat inputRotateCopy = inputRotate.clone();
        dicomImageProcessor.dicomImageRotateCorrection(inputRotate);
        EXPECT_EQ(inputRotate.size(), inputRotateCopy.size());
        EXPECT_EQ(inputRotate.type(), inputRotateCopy.type());

        cv::Mat diffRotate;
        cv::bitwise_xor(inputRotate, inputRotateCopy, diffRotate);
        EXPECT_NE(cv::countNonZero(diffRotate), 0);

        cv::Mat inputRotated;
        cv::rotate(inputRotateCopy, inputRotated, cv::ROTATE_180);
        cv::Mat diffRotated;
        cv::bitwise_xor(inputRotate, inputRotated, diffRotated);
        EXPECT_EQ(cv::countNonZero(diffRotated), 0);

}

void simulateRectBlob(cv::Mat input, const size_t& numberOfBlob, const size_t& blobWidth, const size_t& blobHeight)
{



        // Loop to create blobs
        for (int i = 0; i < numberOfBlob; ++i) {
                // Randomly determine the top-left corner of the blob
                int x = rand() % (input.cols - blobWidth);
                int y = rand() % (input.rows - blobHeight);

                // Draw the blob (a filled rectangle in this case)
                rectangle(input, Point(x, y), Point(x + blobWidth, y + blobHeight), Scalar(255), FILLED);
        }
}

TEST(DicomImageProcessorTest, dicomImageIndicatorRoiDetectionTest)
{
        DicomImageProcessorTest dicomImageProcessor;
        Mat indicator = Mat::zeros(320, 640, CV_8UC1);
        size_t numOfBlob = 5;
        size_t width = 15;
        size_t height = 15;
        size_t minArea = (width + 1) * (height + 1) - 1;
        size_t maxArea = (width + 1) * (height + 1) + 1;
        simulateRectBlob(indicator, numOfBlob, width, height);
        vector<Rect> blob(dicomImageProcessor.findIndicatorRoi(indicator, minArea, maxArea, 0.99f, 1.01f, 0.99f));
        EXPECT_EQ(blob.size(), numOfBlob);
        for (auto& rect : blob) {
                EXPECT_GE(rect.area(), minArea);
                EXPECT_LE(rect.area(), maxArea);
                EXPECT_GE(rect.width / rect.height, 0.99f);
                EXPECT_LE(rect.width / rect.height, 1.01f);
        }

}


// Main function
int main(int argc, char** argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}