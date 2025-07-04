"""
pyvizionsdkcapture.py - Demonstrates how to use pyvizionsdk to control ISP camera brightness

Opens a video window with live camera stream.  In the window, keys can be pressed to
increase or decrease the brightness.

 u: increase
 d: decrease
 q: stop playing video and quit program
"""

import pyvizionsdk
from pyvizionsdk import VX_ISP_IMAGE_PROPERTIES, VX_IMAGE_FORMAT

import cv2
import numpy as np

import platform

def __draw_label(img, text, pos):
    font_face = cv2.FONT_HERSHEY_SIMPLEX
    scale = 0.5
    color = (255, 255, 255)
    thickness = cv2.FILLED
    margin = 5
    txt_size = cv2.getTextSize(text, font_face, scale, thickness)

    end_x = pos[0] + txt_size[0][0] + margin
    end_y = pos[1] - txt_size[0][1] - margin
    start_x = pos[0] - margin
    start_y = pos[1] + margin

    cv2.rectangle(img, (start_x, start_y), (end_x, end_y), (0,0,0), thickness)
    cv2.putText(img, text, pos, font_face, scale, color, 1, cv2.LINE_AA)

# Get the camera list
result, camera_list = pyvizionsdk.VxDiscoverCameraDevices()
if result < 1:
    print('No cameras discovered.')
    quit()

# Print camera_list
print("Device List:\n")
for camera in camera_list:
    print(camera)

# Choose the first camera device
idx = 0
camera = pyvizionsdk.VxInitialCameraDevice(idx)

# Open camera
result = pyvizionsdk.VxOpen(camera)
print("Open camera return code:", result)

# Get format
result, format_list = pyvizionsdk.VxGetFormatList(camera)
print("Get format return code:", result)

mjpg_format = None
min_resolution = float('inf')
for format in format_list:
    # Find the minimum resolution in MJPG format
    if format.format == VX_IMAGE_FORMAT.VX_IMAGE_FORMAT_MJPG:
        resolution = format.width * format.height
        if resolution < min_resolution:
            min_resolution = resolution
            mjpg_format = format

# Set format
result = pyvizionsdk.VxSetFormat(camera, mjpg_format)

min_width = mjpg_format.width
min_height = mjpg_format.height

print(f"Video resolution: ", min_width, "x", min_height)

# Capture image with opencv VideoCapture API
if platform.system() == 'Windows':
    cap = cv2.VideoCapture(idx, cv2.CAP_DSHOW)
else:
    cap = cv2.VideoCapture(idx)

cap.set(cv2.CAP_PROP_FRAME_WIDTH, min_width)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, min_height)

# Get the brightness range
result, min_brightness, max_brightness, step, def_brightness = pyvizionsdk.VxGetISPImageProcessingRange(camera, VX_ISP_IMAGE_PROPERTIES.ISP_IMAGE_BRIGHTNESS)
print(f"Brightness min: {min_brightness}, max: {max_brightness}, step: {step}, default: {def_brightness}, return code: {result}")

# Get the brightness
result, brightness, flag = pyvizionsdk.VxGetISPImageProcessing(camera, VX_ISP_IMAGE_PROPERTIES.ISP_IMAGE_BRIGHTNESS)
print(f"Initial ISP brightness: {brightness}, flag: {flag}, return code: {result}")

# Set the brightness
print(f"Playing video with brightness {brightness}, press q in window to exit...")

# Capture video and display
while True:
    ret, frame = cap.read()
    if ret:
        __draw_label(frame, f"brightness: {brightness}", (20,20))
        __draw_label(frame, "u: increase, d: decrease, q: exit", (20,40))
        cv2.imshow(f"Adjust brightness", frame)
        key = cv2.waitKey(1) & 0xFF
        if key == ord('u'):
            if brightness < max_brightness:
                brightness = brightness + step
            result = pyvizionsdk.VxSetISPImageProcessing(camera, VX_ISP_IMAGE_PROPERTIES.ISP_IMAGE_BRIGHTNESS, brightness)
            print(f"Set brightness to: {brightness}")
        if key == ord('d'):
            if brightness > min_brightness:
                brightness = brightness - step
            result = pyvizionsdk.VxSetISPImageProcessing(camera, VX_ISP_IMAGE_PROPERTIES.ISP_IMAGE_BRIGHTNESS, brightness)
            print(f"Set brightness to: {brightness}")
        if key == ord('q'):
            break
    else:
        print("Can't receive frame (stream end?). Exiting ...")
        break       

cap.release()
cv2.destroyAllWindows()

# close camera
pyvizionsdk.VxClose(camera)