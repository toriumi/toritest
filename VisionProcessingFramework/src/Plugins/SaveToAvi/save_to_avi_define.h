/**
 * @file      save_to_avi_define.h
 * @brief     Definition of value for SaveToAvi plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SAVE_TO_AVI_DEFINE_H_
#define _SAVE_TO_AVI_DEFINE_H_

/* Information of plugin.*/
#define PLUGIN_NAME "SaveToAvi"
#define DIPLAY_NAME "Save to avi"

/* Identification ID of UI.*/
#define WND_ID 90000
#define TEXT_AVI_FILE_PATH_ID 90001
#define BTN_SELECT_AVI_FILE_WND_ID 90002
#define STATIC_TEXT_AVI_FILE_PATH_ID 90003
#define BTN_APPLY_ID 90004
#define kSliderFpsId 90005

/* GUI*/
#define WND_TITLE "Save to avi"
#define WND_POINT_X 0
#define WND_POINT_Y 0
#define WND_SIZE_W 300
#define WND_SIZE_H 200

/* Open AVI file button */
#define BTN_SELECT_AVI_FILE_WND_TEXT "Create AVI file ..."
#define BTN_SELECT_AVI_FILE_WND_POINT_X 10
#define BTN_SELECT_AVI_FILE_WND_POINT_Y 80
#define BTN_SELECT_AVI_FILE_WND_SIZE_W 150
#define BTN_SELECT_AVI_FILE_WND_SIZE_H 30

/* Static Text File path  */
#define STATIC_TEXT_AVI_FILE_PATH_POINT_X 10
#define STATIC_TEXT_AVI_FILE_PATH_POINT_Y 120
#define STATIC_TEXT_AVI_FILE_PATH_SIZE_W 25
#define STATIC_TEXT_AVI_FILE_PATH_SIZE_H 25

/* Text CtrlFile path  */
#define TEXT_AVI_FILE_PATH_POINT_X STATIC_TEXT_AVI_FILE_PATH_POINT_X + 30
#define TEXT_AVI_FILE_PATH_POINT_Y 120
#define TEXT_AVI_FILE_PATH_SIZE_W 240
#define TEXT_AVI_FILE_PATH_SIZE_H 25

#define BTN_SELECT_AVI_FILE_WND_POINT_X 10
#define BTN_SELECT_AVI_FILE_WND_POINT_Y 80
#define BTN_SELECT_AVI_FILE_WND_SIZE_W 150
#define BTN_SELECT_AVI_FILE_WND_SIZE_H 30

/* Apply button */
#define BTN_APPLY_TEXT "Apply"
#define BTN_APPLY_POINT_X 190
#define BTN_APPLY_POINT_Y 150
#define BTN_APPLY_SIZE_W 80
#define BTN_APPLY_SIZE_H 30

#define QUEUE_SIZE 10
typedef cv::Mat* data_t;

#define kDiffTimeCheck 10000
#define kFrameCountNum 10
#define kPutTextPointW 2
#define kPutTextPointH 28
#define kScalarMax 255

#define kVideoWriterNoCodec 0
#define kVideoWriterDefaultFps 60

#define kSaveToAviConfigFile "../lib/Plugins/output/SaveToAvi.ini"

#endif /* _SAVE_TO_AVI_DEFINE_H_*/
