"""
setehdr.py - Demonstrates how to use pyvizionsdk to control ISP camera brightness

Opens a video window with live camera stream.  In the window, a keys can be pressed to
increase or decrease the number of exposures used for eHDR.

Reads the the exposure time and gain, and displays on video window.

This script only works with cameras that have eHDR functionality:
 AR0821
 AR0822

Keys:
 e: increase the maximum number of exposures for eHDR (from 1 to 4 depending on the camera).
    When it reaches the maximum, it rolls over to 1 exposure.
 q: stop playing video and quit program
"""
import pyvizionsdk
from pyvizionsdk import VX_ISP_IMAGE_PROPERTIES, VX_IMAGE_FORMAT
import cv2

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
print("Discovered cameras:")
for camera in camera_list:
    print(camera)

# Choose the first camera device
idx = 0
camera = pyvizionsdk.VxInitialCameraDevice(idx)

# open camera
result = pyvizionsdk.VxOpen(camera)
print("Open camera return code:", result)

min_width = 640
min_height = 480

print(f"Video resolution: ", min_width, "x", min_height)

# Capture image with opencv VideoCapture API
if platform.system() == 'Windows':
    cap = cv2.VideoCapture(idx, cv2.CAP_DSHOW)
else:
    cap = cv2.VideoCapture(idx)

cap.set(cv2.CAP_PROP_FRAME_WIDTH, min_width)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, min_height)

# get the eHDR info
result, min_ehdr, max_ehdr, step_ehdr, def_ehdr = pyvizionsdk.VxGetISPImageProcessingRange(camera, VX_ISP_IMAGE_PROPERTIES.ISP_EHDR_EXPOSURE_MAX_NUMBER)
print(f"eHDR min: {min_ehdr}, max: {max_ehdr}, step: {step_ehdr}, default: {def_ehdr}, return code: {result}")
ehdr_exposures = def_ehdr
print(f"Playing video with eHDR exposures = {ehdr_exposures}, press q in window to exit...")

# Capture video and display
while True:
    ret, frame = cap.read()
    if ret:
        ret, gain = pyvizionsdk.VxGetCurrentGain(camera)
        ret, exposure = pyvizionsdk.VxGetCurrentExposure(camera)
        __draw_label(frame, f"ehdr_exposures: {ehdr_exposures}", (20,60))
        __draw_label(frame, "e: increase exposures, q: exit", (20,20))
        __draw_label(frame, f"gain: {gain}", (20,80))
        __draw_label(frame, f"exposure (us): {exposure}", (20,100))
        cv2.imshow(f"Adjust eHDR Exposures", frame)
        key = cv2.waitKey(1) & 0xFF
        if key == ord('e'):
            if ehdr_exposures < max_ehdr:
                ehdr_exposures = ehdr_exposures + step_ehdr
            else:
                ehdr_exposures = min_ehdr
            result = pyvizionsdk.VxSetISPImageProcessing(camera, VX_ISP_IMAGE_PROPERTIES.ISP_EHDR_EXPOSURE_MAX_NUMBER, ehdr_exposures)
            print(f"Set max ehdr exposures : {ehdr_exposures}")
        if key == ord('q'):
            break
    else:
        print("Can't receive frame (stream end?). Exiting ...")
        break       

cap.release()
cv2.destroyAllWindows()
# close camera
pyvizionsdk.VxClose(camera)