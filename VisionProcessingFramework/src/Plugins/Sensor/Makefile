# Makefile
TARGETS = Sensor.so
OBJS = $(SRCS:.cpp=.o)
CC = g++

SSP_LIB = -L ../../../config/sensor/lib
SSP_INC = -I include

CFLAGS = -shared -fPIC -Wall -rdynamic
SRCS = $(wildcard *.cpp)
BASE_SRCS = ${wildcard ../../base/*.cpp}
BASE_INC = -I ../../base
OPT = -lm -O3
OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_core
MMAL_LIB = -L/opt/vc/lib -lmmal -lmmal_core -lmmal_util -lvcos -lbcm_host -lwiringPi -lpthread
OBJ_PATH = $(wildcard *.o)
PLGIN_LIB1 = -lssp
PLGIN_LIB2 = -lsspprof


$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS)  -g -o $(TARGETS) $(BASE_SRCS) $(SRCS) $(BASE_INC) $(OPT) $(OPENCV_LIB) $(SSP_LIB) $(MMAL_LIB) $(PLGIN_LIB1) $(PLGIN_LIB2) $(SSP_INC) `wx-config --cxxflags` `wx-config --libs`
#	$(CC) $(CFLAGS) -o $(TARGETS) $(SRCS) $(BASE_INC) $(OPT) $(OPENCV_LIB)  $(SSP_INC) $(SSP_LIB) $(MMAL_LIB) `wx-config --cxxflags` `wx-config --libs`

.cpp.o:
	$(CC) $(CFLAGS) -c $(SSP_INC) $(BASE_INC) $(OPT) $< `wx-config --cppflags`

.PHONY: clean
clean:
	$(RM) *~ $(OBJ_PATH) $(TARGETS)
