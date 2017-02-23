/**
 * @file      sensor_focus_define.h
 * @brief     Definition of values for SensorFocus plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SENSOR_FOCUS_DEFINE_H_
#define _SENSOR_FOCUS_DEFINE_H_

/* Information of plugin.*/
#define kPluginName "SensorFocusOIS"

/* Identification ID of UI.*/
#define kWndId 70000
#define kRboxFocusModeId 70001
#define kButtonFocusModeApplyId 70002
#define kRboxOISModeId 70003
#define kButtonOISModeApplyId 70004
#define kButtonInitId 70005
#define kStaticTextFocusCodeId 70006
#define kTextFocusCodeId 70007
#define kButtonDemoId 70008
#define kButtonOISModeDemoStartId 70009
#define kButtonOISModeDemoStopId 70010

/* GUI*/
#define kWndTitle "SensorFocusOIS"
#define kWndPointX 0
#define kWndPointY 0
#define kWndSizeW 380
#define kWndSizeH 430

/* Init button definition*/
#define kButtonInitName "Init"
#define kButtonInitPointX 110
#define kButtonInitPointY 10
#define kButtonInitSizeX 80
#define kButtonInitSizeY 40

/* Demo Init button definition*/
#define kButtonDemoName "[OIS Mode]Demo Init"
#define kButtonDemoPointX 190
#define kButtonDemoPointY 10
#define kButtonDemoSizeX 160
#define kButtonDemoSizeY 40

/* Focus Mode Radio box*/
#define kRadioFocusModeText "[Focus Mode]"
#define kRadioFocusModePointX 10
#define kRadioFocusModePointY 40
#define kRadioFocusModeSizeW 340
#define kRadioFocusModeSizeH 120

/* Focus Code static text definition*/
#define kStaticTextFocusCodeName "FocusCode"
#define kStaticTextFocusCodePointX 170
#define kStaticTextFocusCodePointY 70
#define kStaticTextFocusCodeSizeW 100
#define kStaticTextFocusCodeSizeH 40

/* Focus Code text ctrl definition*/
#define kTextFocusCodePointX 260
#define kTextFocusCodePointY 60
#define kTextFocusCodeSizeW 80
#define kTextFocusCodeSizeH 40

/* Apply button definition*/
#define kButtonFocusModeApplyName "Apply"
#define kButtonFocusModeApplyPointX 260
#define kButtonFocusModeApplyPointY 110
#define kButtonFocusModeApplySizeX 80
#define kButtonFocusModeApplySizeY 40

/* OIS Mode Radio box */
#define kRadioOISModeText "[OIS Mode]"
#define kRadioOISModePointX 10
#define kRadioOISModePointY 180
#define kRadioOISModeSizeW 340
#define kRadioOISModeSizeH 250

/* Apply button definition*/
#define kButtonOISModeApplyName "Apply"
#define kButtonOISModeApplyPointX 260
#define kButtonOISModeApplyPointY 340
#define kButtonOISModeApplySizeX 80
#define kButtonOISModeApplySizeY 40

/* OIS Demo Start button definition*/
#define kButtonOISModeDemoStartName "Demo Start"
#define kButtonOISModeDemoStartPointX 120
#define kButtonOISModeDemoStartPointY 380
#define kButtonOISModeDemoStartSizeX 110
#define kButtonOISModeDemoStartSizeY 40

/* OIS Demo Stop button definition*/
#define kButtonOISModeDemoStopName "Demo Stop"
#define kButtonOISModeDemoStopPointX 230
#define kButtonOISModeDemoStopPointY 380
#define kButtonOISModeDemoStopSizeX 110
#define kButtonOISModeDemoStopSizeY 40

typedef enum {
  kFocusModeDefault = 0,
  kFocusMode1 = kFocusModeDefault,
  kFocusMode2,
  kFocusMode3,
} FocusModeType;

typedef enum {
  kOisModeDefault = 0,
  kOisModeMode1 = kOisModeDefault,
  kOisModeMode2,
  kOisModeMode3,
  kOisModeMode4,
  kOisModeMode5,
  kOisModeMode6,
  kOisModeMode7,
} OisModeType;

#define kImx378FocusModeInfFile \
  "../config/sensor/register_settings/param/IMX378_FocusMode_Inf.csv"
#define kImx378FocusModeMacroFile \
  "../config/sensor/register_settings/param/IMX378_FocusMode_Macro.csv"

#endif /* _SENSOR_FOCUS_DEFINE_H_*/
