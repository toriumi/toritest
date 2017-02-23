/**
 * @file      plugin_edit_canvas_define.h
 * @brief     Header for PluginEditCanvas class definition
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _PLUGIN_EDIT_CANVAS_DEFINE_H_
#define _PLUGIN_EDIT_CANVAS_DEFINE_H_

#define kEditCanvasScreenWidth 50
#define kEditCanvasScreenHeight 50

#define kAddPluginLineLength 50
#define kIconIntervalW 30
#define kIconIntervalH 120

#define kFirstPluginStartX -20
#define kFirstPluginStartY 50

#define kAddPluginIconW 35
#define kAddPluginIconH 35
#define kPluginIconW 25

#define kOnePluginPaneWidth \
  (kAddPluginIconW + kIconIntervalW * 2 + kPluginIconW * 2)
#define kOnePluginPaneHeight 120

#define kCycleTextOffsetX 3
#define kCycleTextOffsetY 1

#define kMaxRawPluginNum 10
#define kMaxColPluginNum 5

#define kButtonAddId 10000
#define kButtonBranchId 10001
#define kButtonDeleteId 10002
#define kListBoxPluginId 10003
#define kButtonApplyId 10004
#define kButtonReplaceId 10005
#define kButtonSettingId 10006

#define kBitmapPlusPath "../config/plus.bmp"
#define kBitmapPlusSelectedPath "../config/plus_selected.bmp"
#define kBitmapPlusInvalidPath "../config/plus_invalid.bmp"
#define kBitmapPluginPath "../config/plugin.bmp"

/* FixCanvas start */
typedef enum {
  kNotSetPlugin = 0,
  kSetBlankPlugin,
  kSetValidPlugin,
  kBranchPointDummyPlugin,
  kBranchLineDummyPlugin,
  kDeleteDummyPlugin
} PluginSettingState;

typedef enum {
  kNotConnectPlugin,
  kConnectedPlugin,
  kCannotConnectPlugin
} PluginConnectionState;
/* FixCanvas end */

typedef enum {
  kFlowActionInvalid = 0,
  kFlowActionAddNext,
  kFlowActionAddBranch,
  kFlowActionReplace,
  kFlowActionDelete,
  kFlowActionOpenSettingWnd
} FlowActionType;

typedef enum {
  kPluginMenuEventNone = 0,
  kPluginMenuEventReplacePressed
} PluginMenuEventType;

typedef enum {
  kPlusMenuEventNone = 0,
  kPlusMenuEventAddPressed,
  kPlusMenuEventBranchPressed,
  kPlusMenuEventDeletePressed
} PlusMenuEventType;

typedef enum {
  kDisableAddPlugin = 0x00,
  kEnableToAddPrevPlugin = 0x10,
  kEnableToAddNextPlugin = 0x20,
  kEnableToAddSubNextPlugin = 0x40,
  kDisableToAddPrevPlugin =
      (kEnableToAddNextPlugin | kEnableToAddSubNextPlugin),
  kDisableToAddNextPlugin = (kEnableToAddPrevPlugin),
  kEnableToAddAllPlugin = (kEnableToAddPrevPlugin | kEnableToAddNextPlugin |
                           kEnableToAddSubNextPlugin)
} PluginAddEnableState;

/* Save and Load plugin flow */
#define kMainFlowString "[main_flow]"
#define kSubFlowString "[sub_flow]"
#define kFlowSettingString "[settings]"
#define kFlowCycleString "[cycle]"

#endif /* _PLUGIN_EDIT_CANVAS_DEFINE_H_*/
