cmake_minimum_required(VERSION 3.14)
project(VizionSDKOpenCV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vizionsdk package
find_package(vizionsdk REQUIRED)

# code src directory path
set(SOURCES "${CMAKE_SOURCE_DIR}")

# Add executable
add_executable(VizionCapture ${SOURCES}/captureImg.cpp)
add_executable(VizionDisplay ${SOURCES}/displayImg.cpp)
add_executable(VizioneHDR ${SOURCES}/setehdr.cpp)
add_executable(VizionSave ${SOURCES}/saveImg.cpp)

# find opencv library
if(WIN32)
    set(OpenCV_DIR "D:/opencv/build/x64/vc16/lib")
    list(APPEND CMAKE_PREFIX_PATH ${OpenCV_DIR})
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS}) 
elseif(UNIX)
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})   
endif()

target_link_libraries(VizionCapture
PRIVATE
vizionsdk::VizionSDK
${OpenCV_LIBS}
)
target_link_libraries(VizionDisplay
PRIVATE
vizionsdk::VizionSDK
${OpenCV_LIBS}
)
target_link_libraries(VizioneHDR
PRIVATE
vizionsdk::VizionSDK
${OpenCV_LIBS}
)
target_link_libraries(VizionSave
PRIVATE
vizionsdk::VizionSDK
${OpenCV_LIBS}
)