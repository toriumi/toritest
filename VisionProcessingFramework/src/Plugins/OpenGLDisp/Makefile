# Makefile
TARGETS = OpenGLDisp.so
OBJS = $(SRCS:.cpp=.o)
CC = g++
#CFLAGS = -shared -fPIC -Wall -rdynamic
SRCS = $(wildcard *.cpp)
BASE_SRCS = ${wildcard ../../base/*.cpp}
BASE_INC = -I ../../base
#OPT = -lm -std=c++11
OPT = -lm -lX11 -O3
OPENCV_LIB = -L/usr/lib/arm-linux-gnueabihf -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -lopencv_core
OBJ_PATH = $(wildcard *.o)

CFLAGS = -shared -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi -rdynamic
LDFLAGS+=-L$(SDKSTAGE)/opt/vc/lib/ -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm -L$(SDKSTAGE)/opt/vc/src/hello_pi/libs/ilclient -L$(SDKSTAGE)/opt/vc/src/hello_pi/libs/vgfont
INCLUDES+=-I$(SDKSTAGE)/opt/vc/include/ -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux -I./ -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/ilclient -I$(SDKSTAGE)/opt/vc/src/hello_pi/libs/vgfont


$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -g -fopenmp -o $(TARGETS) $(BASE_SRCS) $(SRCS) $(LDFLAGS) $(BASE_INC) $(OPT) $(OPENCV_LIB) `wx-config --cxxflags` `wx-config --libs` -Wl,--no-whole-archive -rdynamic
#	$(CC) $(CFLAGS) -g -fopenmp -o $(TARGETS) $(BASE_SRCS) $(SRCS) $(BASE_INC) $(OPT) $(OPENCV_LIB) `wx-config --cxxflags` `wx-config --libs`
#	$(CC) $(CFLAGS) -o $(TARGETS) $(SRCS) $(BASE_INC) $(OPT) $(OPENCV_LIB) `wx-config --cxxflags` `wx-config --libs`

.cpp.o:
#	$(CC) $(CFLAGS) -fopenmp -c $(BASE_INC) $(OPT) $< `wx-config --cppflags`
	$(CC) $(CFLAGS) $(INCLUDES) -fopenmp -c $(BASE_INC) $(OPT) $< `wx-config --cppflags` -o $@ -Wno-deprecated-declarations

.PHONY: clean
clean:
	$(RM) *~ $(OBJ_PATH) $(TARGETS)
