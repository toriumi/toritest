/**
* @file      include.h
* @brief     Header for Common Include
* @author    Vision Processing Community
* @copyright 2016 Vision Processing Community. All rights reserved.
*/

#ifndef _INCLUDE_
#define _INCLUDE_

#include <opencv/cv.h>
#include <stdio.h>
#include <sys/time.h>
#include <wx/bmpbuttn.h>
#include <wx/dc.h>
#include <wx/gdicmn.h>
#include <wx/image.h>
#include <wx/panel.h>
#include <wx/region.h>
#include <wx/spinbutt.h>
#include <wx/textfile.h>
#include <wx/thread.h>
#include <wx/timer.h>
#include <wx/tokenzr.h>
#include <wx/wx.h>
#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#include <map>
#include <string>

#include "./porting_rpi.h"

typedef enum {
  kGRAY8 = 0,
  kGRAY16,
  kRGB444,
  kBGR444,
  kRGB555,
  kBGR555,
  kRGB565,
  kBGR565,
  kRGB888,
  kBGR888,
  kRGB101010,
  kBGR101010,
  kARGB,
  kRGBA,
  kABGR,
  kBGRA,
  kRGB48,
  kBGR48,
  kRGBA64,
  kBGRA64,
  kNone,
} PlaneType;

typedef struct {
  PlaneType plane_type;
  int channel;
  int bits_per_pixel;
  int type;
} PlaneTypeInfo;

typedef enum {
  kFileTypeDefMin = -1,
  kFileTypeInvalid,
  kFileTypeRaw,
  kFileTypeJpg,
  kFileTypeBmp,
  kFileTypePng,
  kFileTypeDefMax
} FileType;

typedef enum { kInputPort, kOutputPort } PortType;

typedef enum { kInputPlugin, kIspPlugin, kOutputPlugin } PluginType;

typedef enum {
  kDepth_Char = 0,  // corresponding to CV_8S
  kDepth_Uchar,     // corresponding to CV_8U
  kDepth_Short,     // corresponding to CV_16S
  kDepth_Ushort,    // corresponding to CV_16U
  kDepth_Int,       // corresponding to CV_32S
  kDepth_Float,     // corresponding to CV_32F
  kDepth_Double     // corresponding to CV_64F
} DepthType;

typedef enum { kStop, kRun, kPause } ImageProcessingState;

#endif /* _INCLUDE*/
