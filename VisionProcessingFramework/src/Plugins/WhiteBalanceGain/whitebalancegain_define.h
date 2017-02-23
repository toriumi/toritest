/**
 * @file      whitebalancegain_define.h
 * @brief     Definition of values for WhiteBalanceGain plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _WHITE_BALANCEGAIN_DEFINE_H_
#define _WHITE_BALANCEGAIN_DEFINE_H_

/* Information of plugin.*/
#define kPluginName "WhiteBalanceGain"

/* Identification ID of UI.*/
#define kWndId 70000
#define kDefaultButtonId 70001
#define kOnePushButtonId 70002
#define kApplyButtonId 70003

/* GUI*/
#define kWndTitle "WhiteBalanceGain"
#define kWndPointX 0
#define kWndPointY 0
#define kWndSizeW 370
#define kWndSizeH 160

/* String definition*/
#define kStringRed "Red"
#define kStringGreen "Green"
#define kStringBlue "Blue"
#define kButtonDefault "Default"
#define kStringOnPush "One Push"
#define kStringApply "Apply"

#define kWhiteBalanceGainConfigFile "../lib/Plugins/isp/WhiteBalanceGain.ini"
#define kWhiteBalanceGainDefaultValue 1.00

#endif /* _WHITE_BALANCEGAIN_DEFINE_H_*/
