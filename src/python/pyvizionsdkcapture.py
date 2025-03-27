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

# captured by opencv
cam = cv2.VideoCapture(idx)
# Capture frame
ret, frame = cam.read()
cv2.imshow('Before setting', frame)
cv2.waitKey(0)
cv2.destroyAllWindows()
cam.release()

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
cam = cv2.VideoCapture(idx)
# Capture frame
ret, frame = cam.read()
cv2.imshow('After setting', frame)
cv2.waitKey(0)
cam.release()
cv2.destroyAllWindows()

# close camera
pyvizionsdk.VxClose(camera)