# VizionSDK with OpenCV

- This sample project demonstrates how to use CMake to build and run a C++ project integrating VizionSDK with OpenCV. 
- In addition to the C++ examples, Python and C# examples with usage instructions are included in the documentation.

## **Sample Code Overview**

The following sample programs demonstrate how to use **VizionSDK** (or **pyvizionsdk** / **CSVizionSDK**) with **OpenCV** for various camera and image processing tasks:

### **1. Image Display**  
`displayImg` : Captures images from the camera and displays them in a window using OpenCV’s `imshow` function.

### **2. Adjust Brightness & Capture Video**   
`captureImg` : Demonstrates how to adjust image brightness via VizionSDK and capture video frames using OpenCV’s `VideoCapture`.

### **3. eHDR Configuration**  
`setehdr` : Shows how to configure the **eHDR maximum exposure setting** and capture images using OpenCV’s `VideoCapture`.

> ⚠️ **Warning**  
> - **eHDR features** are only supported on the following devices: 
>   - VCI-AR0821 / VCI-AR0822
>   - VCS-AR0821 / VCS-AR0822  
>   - VLS3-AR0821 / VLS3-AR0822  
>   - VLS-GM2-AR0821 /VLS-GM2-AR0822  
>   - TEVS-AR0821 / TEVS-AR0822
> - Running the eHDR example on unsupported devices may cause **undefined behavior**.  
> - Make sure **eHDR mode** is **enabled** before running the eHDR example.

### **4. Save Images**  
`saveImg` : Captures images from the camera and saves them to disk using OpenCV’s `imwrite` function.

---

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
   - **For C++ Development:**    
    - OpenCV can be downloaded at release page: [OpenCV](https://opencv.org/releases/)
      - **Windows**: Before building the sample code, please ensure the path in [CMakeList.txt](https://github.com/TechNexion-Vision/vizionsdk-opencv/tree/main/cpp/CMakeLists.txt#L19) is correctly set to your local OpenCV installation.
      - **Linux**: 
        To install OpenCV, please follow the steps provided in the official tutorial: [OpenCV installation](https://docs.opencv.org/4.9.0/d7/d9f/tutorial_linux_install.html)   
   - **For Python Development:**   
      Install with:
      ```
      pip install opencv-python
      ```
    - **For C# Development:**  
      Install the OpenCVSharp4 NuGet package for .NET:
      ```bash
      dotnet add package OpenCvSharp4
      ```
      > **Note:**  
      Ensure that the `OpenCvSharp4.runtime.{OS}` package is also installed for your platform OS runtime dependencies.

## Documents
- Github Pages: https://github.com/TechNexion
- VizionSDK Overview: https://developer.technexion.com/docs/vision-software/vizionsdk
- VizionSDK API User Guide: https://developer.technexion.com/docs/category/vizionsdk-api

## Related links
- [Technexion Main Page](https://www.technexion.com/)