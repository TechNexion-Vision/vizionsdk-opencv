"""
displayImg.py - Demonstrates how to use pyvizionsdk to get the image and display the image by OpenCV.
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
mjpg_format = None
min_resolution = float('inf')
for format in format_list:
    # get mjpg format and minimum resolution
    if format.format == VX_IMAGE_FORMAT.VX_IMAGE_FORMAT_MJPG:
        resolution = format.width * format.height
        if resolution < min_resolution:
            min_resolution = resolution
            mjpg_format = format
print("Return code:", result)

# set format
result = pyvizionsdk.VxSetFormat(camera, mjpg_format)

# start streaming
result = pyvizionsdk.VxStartStreaming(camera)

# get the MJPG format image
result, image = pyvizionsdk.VxGetImage(camera, 1000, mjpg_format)

# retrieve the data to opencv and display with cv2.imshow()
nparr = np.frombuffer(image, np.uint8)
image = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
cv2.imshow("MJPG Image", image)
cv2.waitKey(0)
cv2.destroyAllWindows()

# stop streaming
pyvizionsdk.VxStopStreaming(camera)
# close camera
pyvizionsdk.VxClose(camera)