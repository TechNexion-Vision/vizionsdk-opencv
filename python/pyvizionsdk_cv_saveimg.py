"""
pyvizionsdk_cv_saveimg.cpp - Demonstrates how to use vizionsdk to get the image and save the image by OpenCV.
"""

import pyvizionsdk
from pyvizionsdk import VX_IMAGE_FORMAT

import cv2
import numpy as np

result, camera_list = pyvizionsdk.VxDiscoverCameraDevices()
if result < 1:
    print('No cameras discovered.')
    quit()

# print camera_list
print("Device List:\n")
for camera in camera_list:
    print(camera)

# initialize camera device
camera = pyvizionsdk.VxInitialCameraDevice(0)

# open camera
result = pyvizionsdk.VxOpen(camera)
print("Open camera return code:", result)

# get format
result, format_list = pyvizionsdk.VxGetFormatList(camera)
UYVY_format = None
cap_resolution = 1920*1080
for format in format_list:
    # get UYVY format and 1920*1080 resolution
    if format.format == VX_IMAGE_FORMAT.VX_IMAGE_FORMAT_UYVY:
        resolution = format.width * format.height
        if resolution == cap_resolution:
            UYVY_format = format
print("Return code:", result)

# set format
result = pyvizionsdk.VxSetFormat(camera, UYVY_format)

# start streaming
result = pyvizionsdk.VxStartStreaming(camera)

# get the MJPG format image
result, image = pyvizionsdk.VxGetImage(camera, 1000, UYVY_format)

# retrieve the data to opencv and display with cv2.imshow()
nparr = np.frombuffer(image, np.uint8)
uyvy = nparr.reshape((1080, 1920, 2))
bgr = cv2.cvtColor(uyvy, cv2.COLOR_YUV2BGR_UYVY)
cv2.imwrite("capture.png", bgr)

# stop streaming
pyvizionsdk.VxStopStreaming(camera)
# close camera
pyvizionsdk.VxClose(camera)