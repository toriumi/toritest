/**
 * @file      demosaic_define.h
 * @brief     Definition of values for Demosaic plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef __DEMOSAIC_DEFINE_H_
#define __DEMOSAIC_DEFINE_H_

/* Information of plugin.*/
#define kPluginName "Demosaic"

/* Identification ID of UI.*/
#define kWndId 60000
#define kButtonApplyId 60001
#define kNoneId -1

/* GUI*/
#define kWndTitle "Demosaic"
#define kWndPointX 0
#define kWndPointY 0
#define kWndSizeW 150
#define kWndSizeH 200

#define kRadioBoxName "Set color type"
#define kRadioBoxPointX 20
#define kRadioBoxPointY 10

#define kButtonApllyName "Apply"
#define kButtonApllyPointX 10
#define kButtonApllyPointY 150
#define kButtonApllySizeX 130
#define kButtonApllySizeY 30

/* Kind of color */
#define kDemosaicRgb888 0
#define kDemosaicBgr888 1
#define kDemosaicRgb48 2
#define kDemosaicBgr48 3

/* Ini file path */
#define DemosaicConfigFile "../lib/Plugins/isp/Demosaic.ini"

#endif /* __DEMOSAIC_DEFINE_H_*/
