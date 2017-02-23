/**
 * @file      output_disp_opencv_define.h
 * @brief     Definition of value for OutputDispOpencv plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _OUTPUT_DISP_OPENCV_DEFINE_H_
#define _OUTPUT_DISP_OPENCV_DEFINE_H_

/* Information of plugin.*/
#define PLUGIN_NAME "OutputDispOpencv"
#define DIPLAY_NAME "Disp by Opencv"

/* Identification ID of UI.*/
#define WND_ID 90000

/* GUI*/
#define WND_TITLE "Disp by Opencv"
#define WND_POINT_X 0
#define WND_POINT_Y 0
#define WND_SIZE_W 1280
#define WND_SIZE_H 720

typedef cv::Mat* data_t;

#define kDiffTimeCheck 10000
#define kFrameCountNum 10
#define kPutTextPointW 2
#define kPutTextPointH 28
#define kScalarMax 255

#endif /* _OUTPUT_DISP_OPENCV_DEFINE_H_*/
