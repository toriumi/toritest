/**
 * @file      resize_image_define.h
 * @brief     Definition of values for ResizeImage plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _RESIZE_IMAGE_DEFINE_H_
#define _RESIZE_IMAGE_DEFINE_H_

/* Information of plugin.*/
#define kPluginName "ResizeImage"

/* Identification ID of UI.*/
#define kWndId 70000
#define kButtonApplyId 70001
#define kRboxResizeId 70002
#define kRboxHowToResizeId 70003
#define kSliderResizeId 70004
#define kSliderResizeTextId 70005

/* GUI*/
#define kWndTitle "ResizeImage"
#define kWndPointX 0
#define kWndPointY 0
#define kWndSizeW 240
#define kWndSizeH 270

// Resize Type Radio  box
#define kRadioResizeText "Type"
#define kRadioResizePointX 20
#define kRadioResizePointY 20
#define kRadioResizeSizeW 100
#define kRadioResizeSizeH 110

#define kButtonApplyName "Apply"
#define kButtonApplyPointX 40
#define kButtonApplyPointY 140
#define kButtonApplySizeX 80
#define kButtonApplySizeY 30

#define kRadioHowToResizeText "How to Resize"
#define kHowToResizeDefault 0
#define kSliderDefault 100

typedef enum {
  kResizeDouble = 0,
  kResizeDefault,
  kResizeHalf,
  kResizeQuarter,
} ResizeImageType;

#define ResizeConfigFile "../lib/Plugins/isp/ResizeImage.ini"

#endif /* _RESIZE_IMAGE_DEFINE_H_*/
