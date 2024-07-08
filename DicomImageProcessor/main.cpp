#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "DicomImageProcessor.h"
#include <filesystem>
#include <argparse/argparse.hpp>

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
        argparse::ArgumentParser program("DicomImageProcessor");
        program.add_argument("--input")
                .help("Input image path")
                .required();
        program.add_argument("--histogram")
                .help("Generate histogram")
                .default_value(false)
                .implicit_value(true);
        program.add_argument("--method")
                .help("Indicator detection method")
                .default_value(0)
                .scan<'i', int>()
                .required();
        try {
                program.parse_args(argc, argv);
        }
        catch (const std::runtime_error& err) {
                std::cerr << err.what() << std::endl;
                std::cerr << program;
                std::exit(1);
        }
        std::string imgPath = program.get<std::string>("--input");
        int method = program.get<int>("--method");
        bool histogram = program.get<bool>("--histogram");

        for (auto& file : std::filesystem::directory_iterator{ imgPath })  //loop through the current folder
        {
                if (file.path().extension().compare(".png") == 0 ||
                        file.path().extension().compare(".jpeg") == 0 ||
                        file.path().extension().compare(".tif") == 0) {
                        Mat input = imread(file.path().generic_string(), 0);



                        DicomImageProcessor dicomImageProcessor;
                        dicomImageProcessor.dicomImageRotateCorrection(input, DicomImageProcessor::IndicatorDetectionMethod(method));
                        imshow(file.path().filename().string(), input);
                        if (histogram)
                        {
                                imshow(file.path().filename().string() + " histogram", dicomImageProcessor.generateHistogram(input));
                        }
                }
        }
        waitKey(0);
        return 0;
}