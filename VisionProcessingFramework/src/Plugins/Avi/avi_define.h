/**
 * @file      avi_define.h
 * @brief     Definition of values for Avi plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _AVI_DEFINE_H_
#define _AVI_DEFINE_H_

/* Information of plugin.*/
#define PLUGIN_NAME "Avi"
#define DIPLAY_NAME "OpenAviFile"

/* Identification ID of UI.*/
#define kWndId                     90000
#define kBtnSelectAviFileWndId     90004
#define kStaticTextAviFilePathId   90005
#define kTextAviFilePathId         90006
#define kBtnApplyId                90007


/* GUI*/
#define kWndTitle "Open Avi File"
#define kWndPointX 0
#define kWndPointY 0
#define kWndSizeW 220
#define kWndSizeH 120

/* Open AVI file button */
#define kBtnSelectAviFileWndText "Select AVI file ..."
#define kBtnSelectAviFileWndPointX 10
#define kBtnSelectAviFileWndPointY 10
#define kBtnSelectAviFileWndSizeW  150
#define kBtnSelectAviFileWndSizeH  30

/* Static Text File path  */
#define kStaticTextAviFilePathPointX 10
#define kStaticTextAviFilePathPointY 50
#define kStaticTextAviFilePathSizeW 25
#define kStaticTextAviFilePathSizeH 25

/* Text CtrlFile path  */
#define kTextAviFilePathPointX kStaticTextAviFilePathPointX + 30
#define kTextAviFilePathPointY 50
#define kTextAviFilePathSizeW 175
#define kTextAviFilePathSizeH 25

/* Apply button */
#define kBtnApplyText "Apply"
#define kBtnApplyPointX 120
#define kBtnApplyPointY 80
#define kBtnApplySizeW 80
#define kBtnApplySizeH 30

/* Max file path*/
#define kMaxFilePath 128
#define kDefaultFps 60

#define kAviConfigFile "../lib/Plugins/input/Avi.ini"

#endif /* _AVI_DEFINE_H_*/
