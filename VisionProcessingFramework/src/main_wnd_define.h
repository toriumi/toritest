/**
 * @file      main_wnd_define.h
 * @brief     Header for MainWnd class definition
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */
#ifndef _MAIN_WND_DEFINE
#define _MAIN_WND_DEFINE

/* Identification ID of UI.*/
#define kWndId 90000
#define kMenuRawOpenId 10000
#define kMenuRawSaveId 10001
#define kMenuSaveAsImageId 10002
#define kMenuSensorConfigId 10003
#define kMenuPluginManagerId 10004
#define kMenuVersionId 10005
#define kMenuDirectAccessId 10006
#define kMenuDemoOisId 10007
#define kBitmapButtonStartId 10022
#define kBitmapButtonStopId 10023
#define kBitmapButtonPauseId 10024
#define kStaticTextLogId 10025
#define kTextCtrlLogId 10026
#define kMenuAviOpenId 10027

/* UI parameter common definition*/
#define kStaticTextPointY 60
#define kButtonPointY 380
#define kBitmapButtonPointY 0
#define kBitmapButtonSizeW 70
#define kBitmapButtonSizeH 40
#define kStaticTextSizeW 50
#define kStaticTextSizeH 20

/* Main window definition*/
#define kWndTitle "Vision Processing Framework"
#define kWndPointX 410
#define kWndPointY 35
#define kWndSizeW 640
#define kWndSizeH 480

/* Menu bar definition*/
#define kMenuBarFile "File"
#define kMenuBarSensorConfig "Sensor config"
#define kMenuBarPluginManager "Plugin manager"
#define kMenuBarTool "Tool"
#define kMenuBarHelp "Help"

/* Menu definition*/
#define kMenuRawOpen "Raw open"
#define kMenuAviOpen "Avi open"
#define kMenuRawSave "Raw save"
#define kMenuSaveAsImage "Save as image"
#define kMenuSensorConfig "Sensor config"
#define kMenuPluginManager "Plugin manager"
#define kMenuDirectAccess "Direct access"
#define kMenuVersion "Version"
#define kMenuDemoOis "Demo(Focus/OIS)"


/* Start button definition*/
#define kBitmapButtonStartPath "../config/start_button.bmp"
#define kBitmapButtonStartPointX 0
#define kBitmapButtonStartPointY kBitmapButtonPointY
#define kBitmapButtonStartSizeW kBitmapButtonSizeW
#define kBitmapButtonStartSizeH kBitmapButtonSizeH

/* Stop button definition*/
#define kBitmapButtonStopPath "../config/stop_button.bmp"
#define kBitmapButtonStopPointX 70
#define kBitmapButtonStopPointY kBitmapButtonPointY
#define kBitmapButtonStopSizeW kBitmapButtonSizeW
#define kBitmapButtonStopSizeH kBitmapButtonSizeH

/* Pause button definition*/
#define kBitmapButtonPausePath "../config/pause_button.bmp"
#define kBitmapButtonPausePointX 140
#define kBitmapButtonPausePointY kBitmapButtonPointY
#define kBitmapButtonPauseSizeW kBitmapButtonSizeW
#define kBitmapButtonPauseSizeH kBitmapButtonSizeH

/* Log static text definition*/
#define kStaticTextLogName "Log"
#define kStaticTextLogPointX 20
#define kStaticTextLogPointY 60
#define kStaticTextLogSizeW kStaticTextSizeW
#define kStaticTextLogSizeH kStaticTextSizeH

/* Log definition*/
#define kTextCtrlLogName ""
#define kTextCtrlLogPointX 20
#define kTextCtrlLogPointY 90
#define kTextCtrlLogSizeW 700
#define kTextCtrlLogSizeH 300

/* raw open file dialog definition*/
#define kFileDialogRawOpenWndName "Choose a file to open"
#define kFileDialogRaw "*.raw"

#define kSaveAsImageWndTitle "Save As Image"
#define kRawOpenWndTitle "Raw open"

#define kPluginPath "../lib/Plugins"

#endif /* _MAIN_WND_DEFINE*/
