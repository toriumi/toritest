
CC = gcc

SSP_LIB = -L../../lib
SSP_INC = -I../../include

MMAL_LIB = -L/opt/vc/lib -lmmal -lmmal_core -lmmal_util -lvcos -lbcm_host -lwiringPi -lpthread

# for Compute Module
#OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_core
# for Rpi2
OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_core

HEXAVISION_DIR=/home/pi/HexaVision/HexaVisionControl/0.92
HEXAVISION_INC=$(HEXAVISION_DIR)/include
HEXAVISION_LIB=$(HEXAVISION_DIR)/lib

all: stillsample

stillsample: stillsample.c
	$(CC) -o stillsample stillsample.c $(SSP_INC) $(SSP_LIB) $(MMAL_LIB) -lssp -lm -lsspprof -lhexavision_ctl -L$(HEXAVISION_LIB)

clean:
	rm stillsample
