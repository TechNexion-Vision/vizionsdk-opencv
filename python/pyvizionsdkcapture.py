import pyvizionsdk
from pyvizionsdk import VX_UVC_IMAGE_PROPERTIES, VX_IMAGE_FORMAT

import cv2
import numpy as np

import platform

# get the camera list
result, camera_list = pyvizionsdk.VxDiscoverCameraDevices()
print("Return code:", result)
print("Discovered cameras:", camera_list)

# print camera_list
for camera in camera_list:
    print(camera)

# initialize camera device
idx = 0
camera = pyvizionsdk.VxInitialCameraDevice(idx)

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

min_width = mjpg_format.width
min_height = mjpg_format.height

# captured by opencv
if platform.system() == 'Windows':
    cap = cv2.VideoCapture(idx, cv2.CAP_DSHOW)
else:
    cap = cv2.VideoCapture(idx)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, min_width)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, min_height)
# Capture frame
ret, frame = cap.read()
cv2.imshow('Before setting', frame)
cv2.waitKey(0)
cv2.destroyAllWindows()
cap.release()

# get the brightness
result, brightness, flag = pyvizionsdk.VxGetUVCImageProcessing(camera, VX_UVC_IMAGE_PROPERTIES.UVC_IMAGE_BRIGHTNESS)
print("UVC brightness:", brightness)
print("Flag:", flag)
print("Return code:", result)

# set the brightness
result = pyvizionsdk.VxSetUVCImageProcessing(camera, VX_UVC_IMAGE_PROPERTIES.UVC_IMAGE_BRIGHTNESS, 16, 0)
print("Set UVC brightness return code:", result)

# get the brightness
result, brightness, flag = pyvizionsdk.VxGetUVCImageProcessing(camera, VX_UVC_IMAGE_PROPERTIES.UVC_IMAGE_BRIGHTNESS)
print("After setting UVC brightness:", brightness)
print("Flag:", flag)
print("Return code:", result)

# captured by opencv
if platform.system() == 'Windows':
    cap = cv2.VideoCapture(idx, cv2.CAP_DSHOW)
else:
    cap = cv2.VideoCapture(idx)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, min_width)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, min_height)
# Capture frame
ret, frame = cap.read()
cv2.imshow('After setting', frame)
cv2.waitKey(0)
cap.release()
cv2.destroyAllWindows()

# close camera
pyvizionsdk.VxClose(camera)