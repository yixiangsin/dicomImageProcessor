# DICOM Image Processor

This repository contains a DICOM (Digital Imaging and Communications in Medicine) image processing application developed using C++, Visual Studio, and the OpenCV library. The project is designed to provide tools for loading, processing, and analyzing DICOM images commonly used in medical imaging.

## Features

- Load and display DICOM images
- Basic image processing operations (e.g., filtering, contrast adjustment)
- Image analysis tools (e.g., segmentation, edge detection)
- Export processed images in common formats (e.g., PNG, JPEG)
- Cross-platform support via CMake build system

## Requirements

- Visual Studio 2019 or later
- OpenCV 4.10 or later

## Installation

### Windows

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yixiangsin/dicomImageProcessor.git
   git submodule update --init
   cd dicomImageProcessor
   
2. **Install Dependencies**

- Download and install [Visual Studio](https://visualstudio.microsoft.com/downloads/)
- Install [OpenCV](https://opencv.org/releases/) and set up the environment variables ( please set "your Opencv path"\opencv\build\x64\vc16\lib and "your Opencv path"\opencv\build\x64\vc16\bin into system path)

3. **Configure and Build the Project**

- Open Local Folder with Visual Studio
- Select folder you clone the dicomImageProcessor

### Linux

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yixiangsin/dicomImageProcessor.git
   git submodule update --init
   cd dicomImageProcessor
   ```
   
2. **Install Dependencies**
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential cmake libopencv-dev
   ```

3. **Configure and Build the Project**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

## Usage

### Run the Application

### Windows

you will able to run the project after all environment is done setup with visual studio
- Select DicomImageProcessor.cpp
- Edit launch.vs.json in .vs/ folder to add "args": [ "--input", "your dicom image folder Path", "--method", "method to select 0 = edge detection ,1 = pixel threshold, 2 = combine method of both" , "--histogram"]
- Select drop down from the run button and select current document
- Select DicomImageProcessorUnitTest.cpp
- Select drop down from the run button and select current document
- Press LEFT arrow key to navigate to previous image and Press RIGHT arrow key to navigate the next image, Press ESC key to exit

### Linux
for running program
```bash
./DicomImageProcessor/DicomImageProcessor --input "your dicom image folder Path" --method "method to select 0 = edge detection ,1 = pixel threshold , 2 = combine method of both" --histogram
```
-Press LEFT arrow key to navigate to previous image and Press RIGHT arrow key to navigate the next image, Press ESC key to exit
for unit test 
```bash
./UnitTest/DicomImageProcessorUnitTest
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.






