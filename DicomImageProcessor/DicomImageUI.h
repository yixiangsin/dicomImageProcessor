#pragma once
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <deque>
#include "DicomImageProcessor.h"

class DicomImageUI
{
public:
        DicomImageUI(const std::string& foldderpath, const std::vector<std::string>& supportedImageExtensions);
        void run(const int& method, const bool& showHistogram);
private:
        std::deque<std::filesystem::directory_entry> entries;
        DicomImageProcessor dicomImageProcessor;
        const std::vector<std::string>& supportedImageExtensions;
};