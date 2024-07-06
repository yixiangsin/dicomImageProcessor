// DicomImageProcessor.cpp : Defines the entry point for the application.
//
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "DicomImageProcessor.h"
#include <filesystem>
#include <argparse/argparse.hpp>

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

        return cv::Rect();
}

cv::Rect DicomImageProcessor::pixelThresholdDetectionIndicatorRoiDetector(const cv::Mat& input)
{
        return cv::Rect();
}

cv::Rect DicomImageProcessor::featuresDetectionIndicatorRoiDetector(const cv::Mat& input)
{
        return cv::Rect();
}


int main()
{
        for (auto& file : std::filesystem::directory_iterator{ "../../../../dicom/" })  //loop through the current folder
        {
                if (file.path().extension().compare(".jpeg") == 0) {
                        Mat input = imread(file.path().generic_string() , 0);
           
                        imshow("test", input);
                      
                        

                        waitKey(0);
                }
                cout<< file.path() ;    //open the file
                //or because directory_entry is implicit converted to a path, so you can do 
                //std::ifstream fs{ file };
                //... process the file
        }
	return 0;
}


