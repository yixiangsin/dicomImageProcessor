#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "DicomImageProcessor.h"
#include <filesystem>
#include <argparse/argparse.hpp>

using namespace std;
using namespace cv;


int main()
{
        for (auto& file : std::filesystem::directory_iterator{ "../../../../dicom/" })  //loop through the current folder
        {
                if (file.path().extension().compare(".jpeg") == 0) {
                        Mat input = imread(file.path().generic_string(), 0);

                        imshow("test", input);

                        DicomImageProcessor dicomImageProcessor;
                        cv::Rect roi = dicomImageProcessor.indicatorRoiDetector(input, DicomImageProcessor::IndicatorDetectionMethod::EDGE_DETECTION);

                        waitKey(0);
                }
                cout << file.path();
        }
        return 0;
}