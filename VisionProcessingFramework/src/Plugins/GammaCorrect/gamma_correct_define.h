/**
 * @file      gamma_correct_define.h
 * @brief     Definition of values for GammaCorrect plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef __GAMMA_CORRECT_DEFINE_H_
#define __GAMMA_CORRECT_DEFINE_H_

/* Information of plugin.*/
#define kPluginName "GammaCorrect"

/* Identification ID of UI.*/
#define kWndId 60000
#define kRadioButtonFuncId 60001
#define kTextCtrlFuncId 60002
#define kRadioButtonTableId 60003
#define kButtonApplyId 60004
#define kNoneId -1

/* GUI*/
#define kWndTitle "GammaCorrect"
#define kWndPointX 0
#define kWndPointY 0
#define kWndSizeW 240
#define kWndSizeH 190

#define kStaticBoxName "Set curve"
#define kStaticBoxPointX 10
#define kStaticBoxPointY 10
#define kStaticBoxSizeX 220
#define kStaticBoxSizeY 170

#define kRadioButtonFuncName "Function"
#define kRadioButtonFuncPointX 30
#define kRadioButtonFuncPointY 30
#define kRadioButtonFuncSizeX 90
#define kRadioButtonFuncSizeY 50

#define kTextCtrlGammaFuncPointX 130
#define kTextCtrlGammaFuncPointY 40
#define kTextCtrlGammaFuncSizeX 80
#define kTextCtrlGammaFuncSizeY 30

#define kRadioButtonTableName "Table"
#define kRadioButtonTablePointX 30
#define kRadioButtonTablePointY 80
#define kRadioButtonTableSizeX 70
#define kRadioButtonTableSizeY 50

#define kButtonApllyName "Apply"
#define kButtonApllyPointX 20
#define kButtonApllyPointY 130
#define kButtonApllySizeX 200
#define kButtonApllySizeY 30

#define kFileDialogName "Choose a file to open"
#define kFileDialogType "*.csv"

/* Ini file path */
#define GammaCorrectConfigFile "../lib/Plugins/isp/GammaCorrect.ini"

/* Gamma common param (header=2, data=256/1024)*/
#define k8BitTableFileRow 258
#define k10BitTableFileRow 1026

/* Gamma lut table size */
#define k8BitTableRow 256
#define k10BitTableRow 1024

/* Value range */
#define MIN_GAMMA_FUNC_VALUE 0
#define MAX_GAMMA_FUNC_VALUE 50.0

#endif /* __GAMMA_CORRECT_DEFINE_H_*/
