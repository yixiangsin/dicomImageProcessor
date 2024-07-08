#include "DicomImageUI.h"


using namespace std;
using namespace cv;


DicomImageUI::DicomImageUI(const std::string& foldderpath, const std::vector<std::string>& supportedImageExtensions): supportedImageExtensions(supportedImageExtensions)
{
        for (auto& file : std::filesystem::directory_iterator{ foldderpath })
        {
                for (auto& extension : supportedImageExtensions)
                {
                        if (file.path().extension().compare(extension) == 0) {
                                entries.push_back(file);
                                break;
                        }
                }
        }
}

void DicomImageUI::run(const int& method, const bool& showHistogram)
{
        int waitkey = 0;
        auto current = entries.begin();
        do  //loop through the current folder
        {
                auto file = *current;
                Mat input = imread(file.path().generic_string(), 0);



                DicomImageProcessor dicomImageProcessor;
                dicomImageProcessor.dicomImageRotateCorrection(input, DicomImageProcessor::IndicatorDetectionMethod(method));
                imshow(file.path().filename().string(), input);
                if (showHistogram)
                {
                        imshow(file.path().filename().string() + " histogram", dicomImageProcessor.generateHistogram(input));
                }
                waitkey = waitKeyEx(0);
                switch (waitkey)
                {
                case 2424832: //left arrow key
                        if (current != entries.begin())
                        {
                                current--;
                                destroyAllWindows();
                        }
                        break;
                case 2555904: //right arrow key
                        if (current != entries.end() - 1)
                        {
                                ++current;
                                destroyAllWindows();
                        }
                        break;
                }

        } while (waitkey != 27); //esc key
}
