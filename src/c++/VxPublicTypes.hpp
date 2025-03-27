#pragma once
#include <string>
#include <variant>
#include <cstdint>

enum class VX_LOG_LEVEL {
    VX_LOG_LEVEL_TRACE,
    VX_LOG_LEVEL_DEBUG,
    VX_LOG_LEVEL_INFO,
    VX_LOG_LEVEL_WARN,
    VX_LOG_LEVEL_ERROR,
    VX_LOG_LEVEL_CRITICAL,
    VX_LOG_LEVEL_OFF,
};

enum class VX_IMAGE_FORMAT {
    NONE = 0,
    YUY2,
    UYVY,
    NV12,
    MJPG,
};

enum class VX_CAPTURE_RESULT {
    VX_SUCCESS = 0,
    VX_TIMEOUT = -1,
    VX_CAM_OCCUPIED = -2,
    VX_OTHER_ERROR = -3,
    VX_BUFFER_CORRUPTED = -4,
};

struct VxFormat {
    uint8_t mediatypeIdx;
    uint16_t width;
    uint16_t height;
    uint16_t framerate;
    VX_IMAGE_FORMAT format;
};

enum class VX_FX3_FW_TARGET {
    FW_TARGET_NONE = 0, /* Invalid target                   */
    FW_TARGET_RAM,      /* Program firmware (hex) to RAM    */
    FW_TARGET_I2C,      /* Program firmware (hex) to EEPROM */
    FW_TARGET_SPI       /* Program firmware (hex) to SPI    */
};

#pragma pack(push, 1)
struct VxHeaderV2 {  // Header Version 3
    uint8_t headerVersion;
    uint16_t contentOffset;
    uint8_t productName[64];
    uint8_t productVersion;
    uint8_t headInfo[64];
    uint8_t lensVersion;
    uint8_t contentVersion;
    uint32_t contentChecksum;
    uint32_t contentLen;
    uint16_t pllBootdataLen;
};
struct VxHeaderV3 {  // Header Version 3
    // Fixed Area
    uint8_t headerVersion;
    uint16_t contentOffset;
    uint16_t sensorType;
    uint8_t sensorFuseId[16];
    uint8_t productName[64];
    uint8_t lensId[16];
    uint16_t fixChecksum;
    // Dynamic update area
    uint8_t tnFwVersion[2];
    uint16_t vendorFwVersion;
    uint16_t customNumber;  // or revision number
    uint8_t recordYear;
    uint8_t recordMonth;
    uint8_t recordDay;
    uint8_t recordHour;
    uint8_t recordMinute;
    uint8_t recordSecond;
    uint16_t mipiDatarate;
    uint32_t contentLen;
    uint16_t contentChecksum;
    uint16_t totalChecksum;
};
#pragma pack(pop)

enum class VX_CAMERA_INTERFACE_TYPE {
    INTERFACE_USB = 0,
    INTERFACE_MIPI_CSI2,
    INTERFACE_ETHERNET,
};

enum class VX_UVC_IMAGE_PROPERTIES {
    UVC_IMAGE_BRIGHTNESS,
    UVC_IMAGE_CONTRAST,
    UVC_IMAGE_HUE,
    UVC_IMAGE_SATURATION,
    UVC_IMAGE_SHARPNESS,
    UVC_IMAGE_GAMMA,
    UVC_IMAGE_COLORENABLE,
    UVC_IMAGE_WHITEBALANCE,
    UVC_IMAGE_BACKLIGHT_COMPENSATION,
    UVC_IMAGE_GAIN,
    UVC_IMAGE_PAN,
    UVC_IMAGE_TILT,
    UVC_IMAGE_ROLL,
    UVC_IMAGE_ZOOM,
    UVC_IMAGE_EXPOSURE,
    UVC_IMAGE_IRIS,
    UVC_IMAGE_FOCUS,
    UVC_IMAGE_PROP_MAX
};

enum class VX_ISP_IMAGE_PROPERTIES {
    ISP_IMAGE_BRIGHTNESS,
    ISP_IMAGE_CONTRAST,
    ISP_IMAGE_SATURATION,
    ISP_IMAGE_WHITEBALANCE_MODE,
    ISP_IMAGE_WHITEBALANCE_TEMPERATURE,
    ISP_IMAGE_EXPOSURE_MODE,
    ISP_IMAGE_EXPOSURE_TIME,
    ISP_IMAGE_EXPOSURE_MIN_TIME,
    ISP_IMAGE_EXPOSURE_MAX_TIME,
    ISP_IMAGE_EXPOSURE_GAIN,
    ISP_IMAGE_GAMMA,
    ISP_IMAGE_SHARPNESS,
    ISP_IMAGE_BACKLIGHT_COMPENSATION,
    ISP_IMAGE_SPECIAL_EFFECT_MODE,
    ISP_IMAGE_DENOISE,
    ISP_IMAGE_FLIP_MODE,
    ISP_IMAGE_PAN,
    ISP_IMAGE_TILT,
    ISP_IMAGE_ZOOM,
    ISP_IMAGE_FLICK_MODE,
    ISP_IMAGE_JPEG_QUALITY,
    ISP_IMAGE_PROP_MAX
};

enum class I2C_SPEED {
    I2C_SPEED_100K = 100,
    I2C_SPEED_400K = 400,
    I2C_SPEED_1M = 1000,
};

enum class VX_USB_DEVICE_SPEED {
    USB_DEVICE_SPEED_LOW,
    USB_DEVICE_SPEED_FULL,
    USB_DEVICE_SPEED_HIGH,
    USB_DEVICE_SPEED_SUPER,
    USB_DEVICE_SPEED_SUPER_PLUS,
    USB_DEVICE_SPEED_UNKNOWN
};

enum class VX_FLICK_MODE {
    DISABLE,
    _50HZ,
    _60HZ,
    AUTO,
};

enum class VX_AE_MODE_STATUS {
    MANUAL_EXP,
    AUTO_EXP,
    AUTO_GAIN,
};

enum class VX_AWB_MODE_STATUS {
    MANUAL_TEMPERATURE_WB,
    AUTO_WB,
};

enum class VX_FLIP_MODE {
    FLIP_NORMAL,
    FLIP_H_MIRROR,
    FLIP_V_MIRROR,
    FLIP_ROTATE_180,
};

enum class VX_EFFECT_MODE {
    NORMAL_MODE,
    BLACK_WHITE_MODE,
    GRAYSCALE_MODE,
    NEGATIVE_MODE,
    SKETCH_MODE,
};

enum class VX_OSP_PROFILE_FLAG { DISABLED, ENABLED, ENABLED_AND_SAVE };

//* Profile Constants *//
// UVC Profile Name
#define VZCAM_UVC_PROFILE_NAME_EXPOSURE_TIME            "ExposureTime"
#define VZCAM_UVC_PROFILE_NAME_EXPOSURE_GAIN            "ExposureGain"
#define VZCAM_UVC_PROFILE_NAME_EXPOSURE_MODE            "ExposureMode"
#define VZCAM_UVC_PROFILE_NAME_WHITEBALANCE_MODE        "WhiteBalanceMode"
#define VZCAM_UVC_PROFILE_NAME_WHITEBALANCE_TEMPERATURE "WhiteBalanceTemperature"
#define VZCAM_UVC_PROFILE_NAME_BRIGHTNESS               "Brightness"
#define VZCAM_UVC_PROFILE_NAME_CONTRAST                 "Contrast"
#define VZCAM_UVC_PROFILE_NAME_HUE                      "Hue"
#define VZCAM_UVC_PROFILE_NAME_GAMMA                    "Gamma"
#define VZCAM_UVC_PROFILE_NAME_SATURATION               "Saturation"
#define VZCAM_UVC_PROFILE_NAME_SHARPNESS                "Sharpness"
#define VZCAM_UVC_PROFILE_NAME_ZOOM                     "Zoom"
#define VZCAM_UVC_PROFILE_NAME_PAN                      "Pan"
#define VZCAM_UVC_PROFILE_NAME_TILT                     "Tilt"
#define VZCAM_UVC_PROFILE_NAME_BACKLIGHT_COMPENSATION   "BacklightCompensation"

// ISP Profile Name
#define VZCAM_ISP_PROFILE_NAME_EXPOSURE_TIME            "ExposureTime"
#define VZCAM_ISP_PROFILE_NAME_EXPOSURE_MIN_TIME        "ExposureMinTime"
#define VZCAM_ISP_PROFILE_NAME_EXPOSURE_MAX_TIME        "ExposureMaxTime"
#define VZCAM_ISP_PROFILE_NAME_EXPOSURE_GAIN            "ExposureGain"
#define VZCAM_ISP_PROFILE_NAME_EXPOSURE_MODE            "ExposureMode"
#define VZCAM_ISP_PROFILE_NAME_WHITEBALANCE_MODE        "WhiteBalanceMode"
#define VZCAM_ISP_PROFILE_NAME_WHITEBALANCE_TEMPERATURE "WhiteBalanceTemperature"
#define VZCAM_ISP_PROFILE_NAME_SHARPNESS                "Sharpness"
#define VZCAM_ISP_PROFILE_NAME_SATURATION               "Saturation"
#define VZCAM_ISP_PROFILE_NAME_CONTRAST                 "Contrast"
#define VZCAM_ISP_PROFILE_NAME_GAMMA                    "Gamma"
#define VZCAM_ISP_PROFILE_NAME_ZOOM                     "Zoom"
#define VZCAM_ISP_PROFILE_NAME_PAN                      "Pan"
#define VZCAM_ISP_PROFILE_NAME_TILT                     "Tilt"
#define VZCAM_ISP_PROFILE_NAME_BACKLIGHT_COMPENSATION   "BacklightCompensation"
#define VZCAM_ISP_PROFILE_NAME_BRIGHTNESS               "Brightness"
#define VZCAM_ISP_PROFILE_NAME_DENOISE                  "Denoise"
#define VZCAM_ISP_PROFILE_NAME_SPECIAL_EFFECT_MODE      "SpecialEffectMode"
#define VZCAM_ISP_PROFILE_NAME_FLIP_MODE                "FlipMode"
#define VZCAM_ISP_PROFILE_NAME_JPEG_QUAL                "JpegQual"
#define VZCAM_ISP_PROFILE_NAME_FLICK_MODE               "FlickMode"
