cmake_minimum_required(VERSION 3.14)
project(VizionSDKOpenCV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define platform options
option(PLATFORM "Choose the platform: Ubuntu, Windows" "")

if(NOT PLATFORM)
    message(FATAL_ERROR "PLATFORM is not set. Please specify PLATFORM as Ubuntu, or Windows.")
endif()

# Construct platform-specific directory path
set(PLATFORM_DIR "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/")

# Check if PLATFORM_DIR exists
if(NOT EXISTS ${PLATFORM_DIR})
    message(FATAL_ERROR "PLATFORM_DIR does not exist: ${PLATFORM_DIR}")
endif()

# code src directory path
set(SOURCES "${CMAKE_SOURCE_DIR}/src/c++/")
# config src directory path
set(CONFIGS "${CMAKE_SOURCE_DIR}/config/")
# Add header files
set(HEADERS ${SOURCES}/VizionSDK.h)

# Add executable
add_executable(VizionCapture ${SOURCES}/vizionsdkcapture.cpp ${HEADERS})
add_executable(VizionOpenCV ${SOURCES}/vizionsdk_opencv.cpp ${HEADERS})

# Add platform-specific configurations
if(WIN32)
    add_definitions(-D_WIN32)
    add_definitions(-DUNICODE)
    set(OpenCV_DIR "D:/opencv/build/x64/vc16/lib")
    list(APPEND CMAKE_PREFIX_PATH ${OpenCV_DIR})
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})

    set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "Configurations" FORCE)
    target_link_libraries(VizionCapture ${PLATFORM_DIR}/VizionSDK.lib ${OpenCV_LIBS})
    target_link_libraries(VizionOpenCV ${PLATFORM_DIR}/VizionSDK.lib ${OpenCV_LIBS})
    if(EXISTS "${PLATFORM_DIR}VizionSDK.dll")
        # Copy the DLL to the build output directory
        add_custom_command(TARGET VizionCapture POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PLATFORM_DIR}/VizionSDK.dll"
            "$<TARGET_FILE_DIR:VizionCapture>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CONFIGS}/VxConfig.conf"
            "$$<TARGET_FILE_DIR:VizionCapture>")
    else()
        message(FATAL_ERROR "VizionSDK.dll not found in ${PLATFORM_DIR}")
   endif()
elseif(UNIX)
    add_definitions(-D_LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OpenCV REQUIRED opencv4)
    include_directories(${OpenCV_INCLUDE_DIRS})
    # Remove opencv_ts from the list
    list(REMOVE_ITEM OpenCV_LIBRARIES opencv_ts)

    include_directories(${PLATFORM_DIR})
    target_link_libraries(VizionCapture ${PLATFORM_DIR}/libVizionSDK.so ${OpenCV_LIBRARIES})
    target_link_libraries(VizionOpenCV ${PLATFORM_DIR}/libVizionSDK.so ${OpenCV_LIBRARIES})
endif()