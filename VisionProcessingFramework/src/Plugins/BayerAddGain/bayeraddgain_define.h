/**
 * @file      bayeraddgain_define.h
 * @brief     Definition of values BayerAddGain plugin
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef __BAYER_ADD_GAIN_DEFINE_H_
#define __BAYER_ADD_GAIN_DEFINE_H_

/* Information of plugin.*/
#define kPluginName "BayerAddGain"

/* Identification ID of UI.*/
#define kWndId 80000
#define kButtonApplyId 80001
#define kStaticBoxBayerAddGainId 80002
#define kStaticTextBayerAddGainId 80003
#define kStaticTextOpticalBlack1Id 80004
#define kStaticTextOpticalBlack2Id 80005
#define kStaticTextAstaId 80006
#define kStaticTextPlusId 80007
#define kTextCtrlBayerAddGainId 80008

/* GUI*/
#define kWndTitle "BayerAddGain"
#define kWndPointX 0
#define kWndPointY 0
#define kWndSizeW 520
#define kWndSizeH 150

#define kStaticBoxBayerAddGainName "All Pixels"
#define kStaticBoxBayerAddGainPointX 10
#define kStaticBoxBayerAddGainPointY 10
#define kStaticBoxBayerAddGainSizeX 500
#define kStaticBoxBayerAddGainSizeY 130

#define kStatictextPixelValueName "( pixel-value - "
#define kStaticTextPixelValuePointX 30
#define kStaticTextPixelValuePointY 45
#define kStaticTextPixelValueSizeX 150
#define kStaticTextPixelValueSizeY 50

#define kStaticTextOpticalBlack1PointX 150
#define kStaticTextOpticalBlack1PointY 45
#define kStaticTextOpticalBlack1SizeX 80
#define kStaticTextOpticalBlack1SizeY 30

#define kStatictextAstaName "    ) * "
#define kStaticTextAstaPointX 220
#define kStaticTextAstaPointY 45
#define kStaticTextAstaSizeX 50
#define kStaticTextAstaSizeY 50

#define kTextCtrlBayerAddGainPointX 280
#define kTextCtrlBayerAddGainPointY 40
#define kTextCtrlBayerAddGainSizeX 80
#define kTextCtrlBayerAddGainSizeY 30

#define kStatictextPlusName " + "
#define kStaticTextPlusPointX 370
#define kStaticTextPlusPointY 45
#define kStaticTextPlusSizeX 50
#define kStaticTextPlusSizeY 50

#define kStaticTextOpticalBlack2PointX 400
#define kStaticTextOpticalBlack2PointY 45
#define kStaticTextOpticalBlack2SizeX 80
#define kStaticTextOpticalBlack2SizeY 30

#define kButtonApplyName "Apply"
#define kButtonApplyPointX 20
#define kButtonApplyPointY 100
#define kButtonApplySizeX 480
#define kButtonApplySizeY 30

#define kBayerAddGainConfigFile "../lib/Plugins/isp/BayerAddGain.ini"
#define kBayerAddGainDefaultValue 1.00

#endif /* __BAYER_ADD_GAIN_DEFINE_H_*/
