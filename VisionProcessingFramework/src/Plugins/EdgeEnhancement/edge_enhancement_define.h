/**
 * @file      edge_enhancement_define.h
 * @brief     Definition of value for EdgeEnhancement plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _EDGE_ENHANCEMENT_DEFINE_H_
#define _EDGE_ENHANCEMENT_DEFINE_H_

/* Information of plugin.*/
#define PLUGIN_NAME "EdgeEnhancement"
#define DIPLAY_NAME "Edge Enhancement"

/* Identification ID of UI.*/
#define WND_ID 90000
#define ST_COEFF_ID 10000
#define TCTRL_COEFF_ID 20000
#define BTN_APPLY_ID 30000

/* GUI*/
#define WND_TITLE "Edge Enhancement"
#define WND_POINT_X 0
#define WND_POINT_Y 0
#define WND_SIZE_W 350
#define WND_SIZE_H 80

#define ST_COEFF_TEXT "Coefficient"
#define ST_COEFF_POINT_X 30
#define ST_COEFF_POINT_Y 30
#define ST_COEFF_SIZE_W 100
#define ST_COEFF_SIZE_H 50

#define TCTRL_COEFF_POINT_X 130
#define TCTRL_COEFF_POINT_Y 30
#define TCTRL_COEFF_SIZE_W 80
#define TCTRL_COEFF_SIZE_H 30

#define BTN_APPLY_TEXT "Apply"
#define BTN_APPLY_POINT_X 240
#define BTN_APPLY_POINT_Y 30
#define BTN_APPLY_SIZE_W 80
#define BTN_APPLY_SIZE_H 30

/* Parameter names.*/
#define PARAM_NAME_COEFF "Coefficient"

/* Parameter range.*/
#define COEFF_DEFAULT 1
#define COEFF_MAX 100
#define COEFF_MIN 0

/* Ini file path */
#define EdgeConfigFile "../lib/Plugins/isp/EdgeEnhancement.ini"

#endif /* _EDGE_ENHANCEMENT_DEFINE_H_*/
