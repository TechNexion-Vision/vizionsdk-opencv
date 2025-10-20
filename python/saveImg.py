"""
saveImg.py - Demonstrates how to use pyvizionsdk to get the image and save the image by OpenCV.
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

print("Choose a format:\n1. UYVY\n2. MJPG(Default)")
fmt_id = int(input("Please enter format (1 or 2): ") or "1")
select_format = VX_IMAGE_FORMAT.VX_IMAGE_FORMAT_UYVY if fmt_id == 1 else VX_IMAGE_FORMAT.VX_IMAGE_FORMAT_MJPG

# get format
result, format_list = pyvizionsdk.VxGetFormatList(camera)
cap_format = None
min_width = 640
min_height = 480
for format in format_list:
    if format.format == select_format:
        resolution = format.width * format.height
        if (format.width * format.height) == (min_width * min_height) :
            cap_format = format

# set format
result = pyvizionsdk.VxSetFormat(camera, cap_format)

# start streaming
result = pyvizionsdk.VxStartStreaming(camera)

# get image
result, image = pyvizionsdk.VxGetImage(camera, 1000, cap_format)

# retrieve the data to opencv and save with cv2.imwrite()
nparr = np.frombuffer(image, np.uint8)

if select_format == VX_IMAGE_FORMAT.VX_IMAGE_FORMAT_UYVY:
    uyvy = nparr.reshape((min_height, min_width, 2))
    bgr = cv2.cvtColor(uyvy, cv2.COLOR_YUV2BGR_UYVY)
    cv2.imwrite("capture.png", bgr)
elif select_format == VX_IMAGE_FORMAT.VX_IMAGE_FORMAT_MJPG:
    image = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
    cv2.imwrite("capture.png", image)

# stop streaming
pyvizionsdk.VxStopStreaming(camera)
# close camera
pyvizionsdk.VxClose(camera)