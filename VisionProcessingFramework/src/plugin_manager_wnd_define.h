/**
 * @file      plugin_manager_wnd_define.h
 * @brief     Header for PluginManagerWnd class definition
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _PLUGIN_MANAGER_WND_DEFINE
#define _PLUGIN_MANAGER_WND_DEFINE

/* Identification ID of UI.*/
#define kWndId 90000
#define kStaticTextPluginListId 10000
#define kStaticBoxPluginListId 10001
#define kStaticTextFlowId 10002
#define kButtonReloadId 10003
#define kButtonCommonId 10004
#define kListBoxPluginListId 10005
#define kStaticTextPluginEditId 10006
#define kButtonSavePluginFlowId 10007
#define kButtonLoadPluginFlowId 10008

/* UI parameter common definition*/
#define kStaticTextPointY 60
#define kButtonPointY 380
#define kButtonSizeW 80
#define kButtonSizeH 40
#define kBitmapButtonPointY 450  // --> warning: "kBitmapButtonPointY" redefined
#define kBitmapButtonSizeW 100   // --> warning: "kBitmapButtonPointY" redefined
#define kBitmapButtonSizeH 40
#define kStaticTextSizeW 50
#define kStaticTextSizeH 20

/* Main window definition*/
#define kWndTitle "Plugin manager"  //--> warning: "kWndTitle" redefined
#define kWndPointX 30               // --> warning: "kWndPointX" redefined
#define kWndPointY 480              // --> warning: "kWndPointY" redefined
#define kWndSizeW 1450              // --> warning: "kWndSizeW" redefined
#define kWndSizeH 430               // --> warning: "kWndSizeH" redefined

/* PluginList static text definition*/
#define kStaticTextPluginListName "Plugin List"
#define kStaticTextPluginListPointX 50
#define kStaticTextPluginListPointY 20
#define kStaticTextPluginListSizeW kStaticTextSizeW
#define kStaticTextPluginListSizeH kStaticTextSizeH

/* PluginList static box definition*/
#define kStaticBoxPluginListName ""
#define kStaticBoxPluginListPointX 40
#define kStaticBoxPluginListPointY 40
#define kStaticBoxPluginListSizeW 300
#define kStaticBoxPluginListSizeH 600

/* PluginList list box definition*/
#define kListBoxPluginListPointX 50
#define kListBoxPluginListPointY 60
#define kListBoxPluginListSizeW 50
#define kListBoxPluginListSizeH 50

/* Reload button definition*/
#define kButtonReloadName "Reload"
#define kButtonReloadPointX 250
#define kButtonReloadPointY 660
#define kButtonReloadSizeW kButtonSizeW
#define kButtonReloadSizeH kButtonSizeH

/* Load button definition*/
#define kButtonCommonName "Settings..."
#define kButtonCommonPointX 50
#define kButtonCommonPointY 660
#define kButtonCommonSizeW 150
#define kButtonCommonSizeH kButtonSizeH

/* Flow static text definition*/
#define kStaticTextFlowName "Flow"
#define kStaticTextFlowPointX 370
#define kStaticTextFlowPointY 20
#define kStaticTextFlowSizeW kStaticTextSizeW
#define kStaticTextFlowSizeH kStaticTextSizeH

#define kWndPluginEditCanvasName "New plugin edit"
#define kWndPluginEditCanvasPointX 370
#define kWndPluginEditCanvasPointY 500
#define kWndPluginEditCanvasSizeW (kStaticTextSizeW * 3)
#define kWndPluginEditCanvasSizeH kStaticTextSizeH

#define kWndPluginEditCanvasWidth 700
#define kWndPluginEditCanvasHeight 700
#define kWndPluginEditCanvasStartX 370
#define kWndPluginEditCanvasStartY 50
#define kWndPluginEditCanvasEndX \
  (kWndPluginEditCanvasStartX + kWndPluginEditCanvasWidth)
#define kWndPluginEditCanvasEndY \
  (kWndPluginEditCanvasStartY + kWndPluginEditCanvasHeight)

#define kStaticTextSaveFlowName "Save Flow"
#define kStaticTextLoadFlowName "Load Flow"
#define kButtonSaveLoadFlowSizeW 100

/* Plugin Directory Path*/
#define kPluginPath "../lib/Plugins"

#endif /* _PLUGIN_MANAGER_WND_DEFINE*/
