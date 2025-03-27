#pragma once
#ifdef _WIN32
#ifdef BUILD_STATIC
#define VIZIONSDK_API
#else
#ifdef VIZIONSDKEXPORTS
#define VIZIONSDK_API __declspec(dllexport)
#else
#define VIZIONSDK_API __declspec(dllimport)
#endif
#endif
#else
#define VIZIONSDK_API
#endif

#ifndef STRINGIFY
#define STRINGIFY(x) #x
#define TOSTRING(x)  STRINGIFY(x)
#endif

#define VIZIONSDK_VERSION_MAJOR 25
#define VIZIONSDK_VERSION_MINOR 04
#define VIZIONSDK_VERSION_PATCH 0
#define VIZIONSDK_VERSION_BUILD 1

// clang-format off
#define VIZIONSDK_VERSION                 \
    TOSTRING(VIZIONSDK_VERSION_MAJOR) "." \
    TOSTRING(VIZIONSDK_VERSION_MINOR) "." \
    TOSTRING(VIZIONSDK_VERSION_PATCH) "." \
    TOSTRING(VIZIONSDK_VERSION_BUILD)
// clang-format on

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "VxPublicTypes.hpp"

class VxCamera;

void VIZIONSDK_API HelloVizionSDK(void);

int VIZIONSDK_API VxSetLogFile(const std::string& folderPath);
void VIZIONSDK_API VxSetLogLevel(VX_LOG_LEVEL level);

/*
    SDK API for VizionSDK
*/
int VIZIONSDK_API VxDiscoverCameraDevices(std::vector<std::string>& devList);
std::shared_ptr<VxCamera> VIZIONSDK_API VxInitialCameraDevice(int devIdx);
int VIZIONSDK_API VxOpen(std::shared_ptr<VxCamera> vxcam);
int VIZIONSDK_API VxClose(std::shared_ptr<VxCamera> vxcam);
int VIZIONSDK_API VxIsVizionCamera(std::shared_ptr<VxCamera> vxcam);
int VIZIONSDK_API VxGetDeviceName(std::shared_ptr<VxCamera> vxcam, std::string& deviceName);
int VIZIONSDK_API VxGetDeviceInterfaceType(std::shared_ptr<VxCamera> vxcam,
                                           VX_CAMERA_INTERFACE_TYPE& type);

/* Generic UVC Camera API */
int VIZIONSDK_API VxGetHardwareID(std::shared_ptr<VxCamera> vxcam, std::string& hwId);
int VIZIONSDK_API VxGetUSBDeviceSpeed(std::shared_ptr<VxCamera> vxcam, VX_USB_DEVICE_SPEED& speed);

/* Vizion Cameras API */
// UVC Reset
int VIZIONSDK_API VxResetUVC(std::shared_ptr<VxCamera> vxcam);

/* Vizion Camera Information */
int VIZIONSDK_API VxGetUSBFirmwareVersion(std::shared_ptr<VxCamera> vxcam, std::string& fwVer);
int VIZIONSDK_API VxGetTEVSFirmwareVersion(std::shared_ptr<VxCamera> vxcam, std::string& fwVer);
int VIZIONSDK_API VxGetSensorUniqueID(std::shared_ptr<VxCamera> vxcam, std::string& uniqueID);

/* Stream Control */
int VIZIONSDK_API VxGetFormatList(std::shared_ptr<VxCamera> vxcam, std::vector<VxFormat>& fmtList);
int VIZIONSDK_API VxSetFormat(std::shared_ptr<VxCamera> vxcam, VxFormat fmt);
int VIZIONSDK_API VxStartStreaming(std::shared_ptr<VxCamera> vxcam);
int VIZIONSDK_API VxStopStreaming(std::shared_ptr<VxCamera> vxcam);
VX_CAPTURE_RESULT VIZIONSDK_API VxGetImage(std::shared_ptr<VxCamera> vxcam,
                                           uint8_t* data,
                                           int* dataSize,
                                           uint16_t timeout);

// DMA Buffer
VX_CAPTURE_RESULT VIZIONSDK_API VxGetImageFd(std::shared_ptr<VxCamera> vxcam,
                                             int* dmaBufferFd,
                                             uint16_t timeout);
int VIZIONSDK_API VxReleaseImageFd(std::shared_ptr<VxCamera> vxcam, int dmaBufferFd);

/* UVC Image Processing */
int VIZIONSDK_API VxSetUVCImageProcessing(std::shared_ptr<VxCamera> vxcam,
                                          VX_UVC_IMAGE_PROPERTIES propId,
                                          long value,
                                          int flag);

// flag (int): Indicates whether prodId is in automatic mode (1) or manual mode (0).
int VIZIONSDK_API VxGetUVCImageProcessing(std::shared_ptr<VxCamera> vxcam,
                                          VX_UVC_IMAGE_PROPERTIES propId,
                                          long& value,
                                          int& flag);
int VIZIONSDK_API VxGetUVCImageProcessingRange(std::shared_ptr<VxCamera> vxcam,
                                               VX_UVC_IMAGE_PROPERTIES propId,
                                               long& min,
                                               long& max,
                                               long& step,
                                               long& def);

/* ISP Image Processing */
int VIZIONSDK_API VxSetISPImageProcessing(std::shared_ptr<VxCamera> vxcam,
                                          VX_ISP_IMAGE_PROPERTIES propId,
                                          int value);
int VIZIONSDK_API VxSetISPImageProcessingDefault(std::shared_ptr<VxCamera> vxcam);

// flag (int): Indicates whether prodId is in automatic mode (1) or manual mode (0).
int VIZIONSDK_API VxGetISPImageProcessing(std::shared_ptr<VxCamera> vxcam,
                                          VX_ISP_IMAGE_PROPERTIES propId,
                                          int& value,
                                          int& flag);
int VIZIONSDK_API VxGetISPImageProcessingRange(std::shared_ptr<VxCamera> vxcam,
                                               VX_ISP_IMAGE_PROPERTIES propId,
                                               int& min,
                                               int& max,
                                               int& step,
                                               int& def);

int VIZIONSDK_API VxGetCurrentGain(std::shared_ptr<VxCamera> vxcam, uint8_t& gain);
int VIZIONSDK_API VxGetCurrentExposure(std::shared_ptr<VxCamera> vxcam, uint32_t& exp);
int VIZIONSDK_API VxGetMaxFPS(std::shared_ptr<VxCamera> vxcam, uint8_t& fps);
int VIZIONSDK_API VxSetMaxFPS(std::shared_ptr<VxCamera> vxcam, uint8_t fps);
int VIZIONSDK_API VxGetThroughPut(std::shared_ptr<VxCamera> vxcam, uint16_t& throughPut);
int VIZIONSDK_API VxSetThroughPut(std::shared_ptr<VxCamera> vxcam, uint16_t throughPut);

/* Profile Config */
int VIZIONSDK_API VxLoadProfileConfig(std::shared_ptr<VxCamera> vxcam, const std::string profile);
int VIZIONSDK_API VxActivateProfileStreaming(std::shared_ptr<VxCamera> vxcam);
int VIZIONSDK_API VxActivateProfileImageProcessing(std::shared_ptr<VxCamera> vxcam);

/* OSP Profile Config */
int VIZIONSDK_API VxGetOSPProfileConfig(std::shared_ptr<VxCamera> vxcam, std::string& profile);
int VIZIONSDK_API VxSetOSPProfileFlag(std::shared_ptr<VxCamera> vxcam, VX_OSP_PROFILE_FLAG flag);
int VIZIONSDK_API VxGetOSPProfileFlag(std::shared_ptr<VxCamera> vxcam, VX_OSP_PROFILE_FLAG& flag);
int VIZIONSDK_API VxResetOSPProfileConfig(std::shared_ptr<VxCamera> vxcam);

/* Timestamp */
int VIZIONSDK_API VxGetTimestamp(std::shared_ptr<VxCamera> vxcam, uint32_t& timestamp);
int VIZIONSDK_API VxResetTimestamp(std::shared_ptr<VxCamera> vxcam);

/* Frame Count */
int VIZIONSDK_API VxGetFrameCount(std::shared_ptr<VxCamera> vxcam, uint8_t& frameCount);
int VIZIONSDK_API VxResetFrameCount(std::shared_ptr<VxCamera> vxcam);