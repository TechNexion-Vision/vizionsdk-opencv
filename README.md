# VizionSDK with OpenCV

- This sample project demonstrates how to use CMake to build and run a C++ project integrating VizionSDK with OpenCV. 
- In addition to the C++ examples, Python example code and usage instructions are also included in the documentation.

## Sample Code Overview

The following sample programs demonstrate key functionalities using VizionSDK (or pyvizionsdk) together with OpenCV:

- **vizionsdk_opencv.cpp / pyvizionsdk_cv.py**  
  Captures images from the camera and displays them using OpenCV's `imshow` function.

- **vizionsdkcapture.cpp / pyvizionsdkcapture.py**  
  Adjusts the image brightness through VizionSDK and captures video frames with OpenCV’s `VideoCapture`.

- **vizionsdk_ehdr.cpp / pyvizionsdk_ehdr.py**  
  Shows how to configure the eHDR maximum exposure number and capture images using OpenCV’s `VideoCapture`.

- **vizionsdk_cv_saveimg.cpp / pyvizionsdk_cv_saveimg.py**  
  Captures images from the camera and saves them using OpenCV's `imsave` function.

## Prerequisites

Before building and running this project, ensure you have the following installed:

1.  **CMake (Version 3.0 or newer):**
    * CMake is required to configure the build.
    * If you don't have it, download and install it from the [official CMake website](https://cmake.org/download/).
    * Make sure the `cmake` executable is in your system's PATH. You can verify by running `cmake --version` in your terminal.

2.  **C++ Compiler (Supporting C++11):**
    * A C++ compiler compatible with the C++11 standard is needed (e.g., GCC, Clang, MSVC). Install one suitable for your platform if necessary (e.g., `build-essential` on Ubuntu/Debian, Visual Studio C++ workload on Windows).

    If you're using Ubuntu, install with: 
    ```
    sudo apt install cmake build-essential
    ```

3. **OpenCV(version 4.9 or newer)**  
  - For C++ Development:  
    - OpenCV can be downloaded at release page: [OpenCV](https://opencv.org/releases/)
      - **Windows**: Before building the sample code, please ensure the path in [CMakeList.txt](https://github.com/TechNexion-Vision/vizionsdk-opencv/tree/main/cpp/CMakeLists.txt#L19) is correctly set to your local OpenCV installation.
      - **Linux**: 
        To install OpenCV, please follow the steps provided in the official tutorial: [OpenCV installation](https://docs.opencv.org/4.9.0/d7/d9f/tutorial_linux_install.html)   
  - For Python Development: Install with `pip install opencv-python`
   
## Building for C++ Project

1. Open Terminal and navigate to the directory of the project
2. Create a build directory: `mkdir build && cd build`
3. Generate build files using CMake:
    ```
    cmake ..
    ```
4. Build the project using the generated build files: 
    - **Windows**: `cmake --build . --config Release`
    - **Linux**: `make`

## Running the execution
> ⚠️ Warning:  
> - eHDR-related properties are only supported on **VCI-AR0821** and **VCI-AR0822**. Running eHDR example on unsupported devices may result in undefined behavior.  
> - Ensure that **eHDR mode** is **enabled** before running the eHDR example.

After successful building the project, the executable will be located in the `build` directory

- **Windows**: Run the executable from the Command Prompt or double-click it in File Explorer.  
- **Linux**: Open a terminal and run it using `./{execution_name}`.

## Running the Python project

- **Use the following commands to run the examples**
    ```
    python python/pyvizionsdk_cv.py
    python python/pyvizionsdkcapture.py
    python python/pyvizionsdk_ehdr.py
    python python/pyvizionsdk_cv_saveimg.py
    ```