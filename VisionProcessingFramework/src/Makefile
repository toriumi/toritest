# Makefile
CC = g++
TARGETS = VisionProcessingFramework
SRCS = $(wildcard *.cpp)
BASE_SRCS = ${wildcard base/*.cpp}
BASE_INC = -I base
#OPT = -ldl -rdynamic -std=c++11
OPT = -ldl -rdynamic
SSP_LIB = -L ../config/sensor/lib
MMAL_LIB = -lmmal -lmmal_core -lmmal_util -lvcos -lbcm_host -lwiringPi -lpthread

#OpenCV library
OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_core

CFLAGS = -shared -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi
LDFLAGS+=-L$(SDKSTAGE)/opt/vc/lib/ -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm -lssp -lsspprof
INCLUDES+=-I$(SDKSTAGE)/opt/vc/include/ -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I./ -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/ilclient -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/vgfont

#$(TARGETS): $(OBJS)
$(TARGETS): $(SRCS) $(BASE_SRCS)
#	$(CC) -g -o $(TARGETS) $(SRCS) $(BASE_INC) $(OPT) $(OPENCV_LIB) `wx-config --cxxflags` `wx-config --libs`
	$(CC) -g -o $(TARGETS) $(BASE_SRCS) $(SRCS) $(BASE_INC) $(LDFLAGS) $(OPT) $(OPENCV_LIB) $(SSP_LIB) $(MMAL_LIB) `wx-config --cxxflags` `wx-config --libs` ./Plugins/Sensor/Sensor.so
#	$(CC) -o $(TARGETS) $(BASE_SRCS) $(SRCS) $(BASE_INC) $(OPT) $(OPENCV_LIB) `wx-config --cxxflags` `wx-config --libs`

.PHONY: clean
clean:
	$(RM) *~ $(TARGETS)

