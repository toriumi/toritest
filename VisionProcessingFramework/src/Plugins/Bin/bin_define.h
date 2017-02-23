/**
 * @file      bin_define.h
 * @brief     Definition of values for Bin plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _BIN_DEFINE_H_
#define _BIN_DEFINE_H_

/* Information of plugin.*/
#define PLUGIN_NAME "Bin"
#define DIPLAY_NAME "OpenRawFile"

/* Identification ID of UI.*/
#define WND_ID 90000
#define ST_BIT_COUNT_ID 90001
#define RBOX_BIT_COUNT_ID 90002
#define RBOX_FIRST_PIXEL_ID 90003
#define BTN_SELECT_RAW_FILE_WND_ID 90004
#define STATIC_TEXT_RAW_FILE_PATH_ID 90005
#define TEXT_RAW_FILE_PATH_ID 90006
#define BTN_APPLY_ID 90007

/* GUI*/
#define WND_TITLE "Open Raw File"
#define WND_POINT_X 0
#define WND_POINT_Y 0
#define WND_SIZE_W 350
#define WND_SIZE_H 200

/* Bit count static text*/
#define ST_BIT_COUNT_TEXT "Bit count"

/* Bit count text ctrl*/
#define RBOX_BIT_COUNT_POINT_X 10
#define RBOX_BIT_COUNT_POINT_Y 10
#define RBOX_BIT_COUNT_SIZE_W 190
#define RBOX_BIT_COUNT_SIZE_H 70

/* First pixel ctrl*/
#define RBOX_FIRST_PIXEL_POINT_X 10
#define RBOX_FIRST_PIXEL_POINT_Y 80
#define RBOX_FIRST_PIXEL_SIZE_W 190
#define RBOX_FIRST_PIXEL_SIZE_H 70

/* Open RAW file button */
#define BTN_SELECT_RAW_FILE_WND_TEXT "Select RAW file ..."
#define BTN_SELECT_RAW_FILE_WND_POINT_X 10
#define BTN_SELECT_RAW_FILE_WND_POINT_Y 170
#define BTN_SELECT_RAW_FILE_WND_SIZE_W 150
#define BTN_SELECT_RAW_FILE_WND_SIZE_H 30

/* Static Text File path  */
#define STATIC_TEXT_RAW_FILE_PATH_POINT_X 10
#define STATIC_TEXT_RAW_FILE_PATH_POINT_Y 210
#define STATIC_TEXT_RAW_FILE_PATH_SIZE_W 25
#define STATIC_TEXT_RAW_FILE_PATH_SIZE_H 25

/* Text CtrlFile path  */
#define TEXT_RAW_FILE_PATH_POINT_X STATIC_TEXT_RAW_FILE_PATH_POINT_X + 30
#define TEXT_RAW_FILE_PATH_POINT_Y 210
#define TEXT_RAW_FILE_PATH_SIZE_W 175
#define TEXT_RAW_FILE_PATH_SIZE_H 25

#define BTN_SELECT_RAW_FILE_WND_POINT_X 10
#define BTN_SELECT_RAW_FILE_WND_POINT_Y 170
#define BTN_SELECT_RAW_FILE_WND_SIZE_W 150
#define BTN_SELECT_RAW_FILE_WND_SIZE_H 30

/* Apply button */
#define BTN_APPLY_TEXT "Apply"
#define BTN_APPLY_POINT_X 120
#define BTN_APPLY_POINT_Y 250
#define BTN_APPLY_SIZE_W 80
#define BTN_APPLY_SIZE_H 30

/* Max file path*/
#define MAX_FILE_PATH 128

/* Bin header information*/
#define BIN_HEADER_WORD_SIZE 2
#define BIN_HEADER_WIDTH_POS 0
#define BIN_HEADER_HEIGHT_POS 1

/* Bit count information */
/* Bayer RAW10 format. Even line begins RGRGRG..., odd line begins GBGBGB ... */
#define SSP_FRAME_BAYER10 0x1

/* Bayer RAW8 format. Even line begins RGRGRG..., odd line begins GBGBGB ... */
#define SSP_FRAME_BAYER8 0x2

/* Psuedo 16bit Bayer format. Even line begins RGRGRG..., */
/* odd line begins GBGBGB ... */
#define SSP_FRAME_BAYER16 0x6

#define BinConfigFile "../lib/Plugins/input/Bin.ini"

#endif /* _BIN_DEFINE_H_*/
