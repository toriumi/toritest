/**
 * @file      demo_ois_wnd_define.h
 * @brief     Header for DemoOisWnd class definition
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.X.
 */

#ifndef _DEMO_OIS_WND_DEFINE
#define _DEMO_OIS_WND_DEFINE

/* Version information window definition*/
#define kDemoOisWndSettingsTitle "Demo(Focus/OIS)"
#define kDemoOisWndPointX 1120
#define kDemoOisWndPointY 35
#define kDemoOisWndSizeW 280
#define kDemoOisWndSizeH 120


/* Identification ID of UI.*/
#define kDemoOisWndId 90000
#define kButtonDemoOisLoadId 10001
#define kButtonDemoOisStartId 10002
#define kButtonDemoOisDemoStartId 10003

/* Load button definition*/
#define kButtonDemoOisLoadName "Load"
#define kButtonDemoOisLoadPointX 10
#define kButtonDemoOisLoadPointY 10
#define kButtonDemoOisLoadSizeW 80
#define kButtonDemoOisLoadSizeH 40

/* Srart button definition*/
#define kButtonDemoOisStartName "Start"
#define kButtonDemoOisStartPointX 100
#define kButtonDemoOisStartPointY 10
#define kButtonDemoOisStartSizeW 160
#define kButtonDemoOisStartSizeH 40

/* Srart button definition*/
#define kButtonDemoOisDemoStartName "OIS Demo Start"
#define kButtonDemoOisDemoStartPointX 100
#define kButtonDemoOisDemoStartPointY 60
#define kButtonDemoOisDemoStartSizeW 160
#define kButtonDemoOisDemoStartSizeH 40

#define kPluginPath "../lib/Plugins"

#define kSensorConfigFilePath "/home/pi/VisionProcessingFramework_FocusOIS_DEMO/config/sensor/640x480_4x4.xml"
#define kFlowFilePath "/home/pi/VisionProcessingFramework_FocusOIS_DEMO/IMX378_OIS.flow"


#endif /* _DEMO_OIS_WND_DEFINE*/
