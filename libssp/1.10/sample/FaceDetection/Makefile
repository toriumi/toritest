CC = gcc

SSP_LIB = -L../../lib
SSP_INC = -I../../include

MMAL_LIB = -L/opt/vc/lib -lmmal -lmmal_core -lmmal_util -lvcos -lbcm_host -lwiringPi -lpthread

# for Compute Module
#OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_core
# for Rpi2
OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_core

CPP_LIB = -lstdc++

HEXAVISION_DIR=/home/pi/HexaVision/HexaVisionControl/0.9
HEXAVISION_INC=$(HEXAVISION_DIR)/include
HEXAVISION_LIB=$(HEXAVISION_DIR)/lib

all: FaceDetection

FaceDetection : FaceDetection.cpp
	$(CC)  -O3 -Wall -o FaceDetection FaceDetection.cpp  -lm $(SSP_INC) $(SSP_LIB) -lssp $(MMAL_LIB) $(OPENCV_LIB) $(CPP_LIB) -lsspprof -lhexavision_ctl -L$(HEXAVISION_LIB)
clean:
	rm FaceDetection
