import pyvizionsdk
from pyvizionsdk import VX_UVC_IMAGE_PROPERTIES

import cv2
import numpy as np

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

result, fmt_list = pyvizionsdk.VxGetFormatList(camera)
min_width = fmt_list[0].width
min_height = fmt_list[0].height
for fmt in fmt_list:
    # find smallest size format
    if (fmt.width * fmt.height < min_width * min_height):
        min_width = fmt.width
        min_height = fmt.height

# captured by opencv
cap = cv2.VideoCapture(idx, cv2.CAP_DSHOW)
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
cap = cv2.VideoCapture(idx, cv2.CAP_DSHOW)
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