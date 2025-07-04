# VizionSDK with OpenCV

- This sample project demonstrates how to use CMake to build and run a C++ project integrating VizionSDK with OpenCV. 
- In addition to the C++ examples, Python example code and usage instructions are also included in the documentation.

## Prerequisites

Before building and running this project, ensure you have the following installed:

### CMake (version 3.0 or newer)
- **Windows**: 
  - You can download the latest CMake installer from the [official CMake website](https://cmake.org/download/) and follow the installation instructions provided.
  - Ensure that CMake is added to your system PATH during installation to enable command-line usage.

- **Linux**: 
  - On Debian-based systems (e.g., Ubuntu), you can install CMake using apt:
    ```
    sudo apt update
    sudo apt install cmake
    ```
  - If sudo apt install cmake is not available. You can manually download and install [CMake](https://cmake.org/download/) from the official website:

    If you're working with NXP, Nvidia, or TI platforms  look for the Linux aarch64 binary distribution. Download the appropriate tar.gz file for your platform.
    1. Extract the downloaded file:
    ```
    tar -zxvf cmake-[version]-linux-aarch64.tar.gz
    ```
    2. Move the extracted directory:
       - Move the extracted files to a directory like /opt for easier management:
         ```
         sudo mv cmake-[version]-linux-aarch64 /opt/cmake
         ```
    3. Update the PATH environment variable:
       - Add CMake to your system PATH by modifying your ~/.bashrc or ~/.profile file:
         ```
          export PATH=/opt/cmake/bin:$PATH
         ```
        - Save the file and reload your profile with
          ```
           source ~/.profile
          ```
### C++ compiler (supporting C++11 standard)

- **Windows**: 
  - For Windows, you might need to install a C++ compiler separately. You can use Microsoft Visual Studio, which includes the MSVC compiler.
    - When installing Visual Studio, make sure to select the "Desktop development with C++" workload during the installation process. This will include the necessary C++ compiler, nmake, and other essential tools for C++ development on Windows. 
  - Alternatively, you can use MinGW, a port of the GNU Compiler Collection (GCC), which provides a C++ compiler for Windows.

- **Linux**: 
    - Most Linux distributions come with a C++ compiler pre-installed. However, if it's not available, you can install it using your package manager.
  On Debian-based systems (e.g., Ubuntu), you can install the GNU Compiler Collection (GCC) using apt:
    ```
    sudo apt update
    sudo apt install build-essential
    ```
   - Embedded systems:
    Embedded systems typically do not require a compiler. Development is done using cross-compilation on a host machine, and the resulting binaries are deployed to the embedded device.

### OpenCV(version 4.9 or newer)

#### For C++ Development

- **Windows**: 
  1. Visit the OpenCV release page: [OpenCV](https://github.com/opencv/opencv/releases)
  2. Download the latest Windows version(opencv-{version}-windows.exe).
  3. Extract it to `C:\opencv` or another location of your choice.
  4. Set the path in [CMakeList.txt](https://github.com/TechNexion-Vision/vizionsdk-opencv/tree/main/CMakeLists.txt#L19) to your opencv location
    ```
    set(OpenCV_DIR "{your_OpenCV_path}/build/x64/vc16/lib")
    ```

- **Linux**: 
  1. Clone the OpenCV packages (version 4.9.0 for instance)
    ```
    git clone --branch 4.9.0 https://github.com/opencv/opencv.git
    ```
  2. Configure OpenCV using CMake
    ```
    cd ~/opencv
    mkdir build && cd build
    cmake ..
    ```
  3. Build and Install OpenCV
    ```
    make -j$(nproc)
    sudo make install
    ```

#### For Python Users

- **Using `pip` install the packages**
    ```
    pip install opencv-python
    ```
   
## Building for C++ Project

### Windows

1. Open Command Prompt or PowerShell.
2. Navigate to the directory of the project:
3. Create a build directory: `mkdir build && cd build`
4. Generate build files using CMake:
  ```
  cmake ..
  ```
5. Build the project using the generated build files: `cmake --build . --config Release`

### Linux

1. Open Terminal.
2. Navigate to the directory of the project:
3. Create a build directory: `mkdir build && cd build`
4. Generate build files using CMake:
  ```
  cmake ..
  ```
5. Build the project using the generated build files: `make`

## Running the execution
> ⚠️ Warning:  
> - eHDR-related properties are only supported on **VCI-AR0821** and **VCI-AR0822**. Running eHDR example on unsupported devices may result in undefined behavior.  
> - Ensure that **eHDR mode** is **enabled** before running the eHDR example.

### Windows

- After successful building, you will find the executable `VizionOpenCV.exe`, `VizionCapture.exe` and `VizioneHDR.exe` in the `build` directory. You can run it from the command line or by double-clicking on it in the File Explorer.

### Linux

- After successful building, you will find the executable `VizionOpenCV`, `VizionCapture` and `VizioneHDR` in the `build` directory. You can run it from the terminal by executing `./{execution_name}`.

## Running the Python project

- **Using `python` to run the python code**
    ```
    python python/pyvizionsdk_cv.py
    python python/pyvizionsdkcapture.py
    python python/pyvizionsdk_ehdr.py
    ```

## Execution Description

- **VizionOpenCV / pyvizionsdk_cv.py** : It's a example for how to get the image by the VizionSDK(or pyvizionsdk) and use OpenCV imshow to display the image.
- **VizionCapture / pyvizionsdkcapture.py** : It's a example for how to adjust the brightness by the VizionSDK(or pyvizionsdk) and use OpenCV VideoCapture to capture the image.
- **VizioneHDR / pyvizionsdk_ehdr.py** : It's a example for how to adjust the eHDR exposure max number by the VizionSDK(or pyvizionsdk) and use OpenCV VideoCapture to capture the image.

   