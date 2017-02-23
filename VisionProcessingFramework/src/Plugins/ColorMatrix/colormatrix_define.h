/**
 * @file      colormatrix_define.h
 * @brief     Definition of values for ColorMatrix plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef __COLOR_MATRIX_DEFINE_H_
#define __COLOR_MATRIX_DEFINE_H_

/* Information of plugin.*/
#define kPluginName "ColorMatrix"

/* Identification ID of UI.*/
#define kWndId 70000
#define kTextCtrlMatrix1st1Id 70001
#define kTextCtrlMatrix1st2Id 70002
#define kTextCtrlMatrix1st3Id 70003
#define kTextCtrlMatrix2nd1Id 70004
#define kTextCtrlMatrix2nd2Id 70005
#define kTextCtrlMatrix2nd3Id 70006
#define kTextCtrlMatrix3rd1Id 70007
#define kTextCtrlMatrix3rd2Id 70008
#define kTextCtrlMatrix3rd3Id 70009
#define kButtonFileDialogId 70010
#define kButtonDefaultId 70011
#define kButtonApplyId 70012
#define kNoneId -1

/* GUI*/
#define kWndTitle "ColorMatrix"
#define kWndPointX 0
#define kWndPointY 0
#define kWndSizeW -1
#define kWndSizeH -1

#define kStaticTextR1Name "R'"
#define kStaticTextR1PointX 20
#define kStaticTextR1PointY 20
#define kStaticTextR1SizeX 25
#define kStaticTextR1SizeY 25

#define kTextCtrlMatrix1st1PointX 70
#define kTextCtrlMatrix1st1PointY 15
#define kTextCtrlMatrix1st1SizeX 80
#define kTextCtrlMatrix1st1SizeY 30
#define kTextCtrlMatrix1st2PointX 155
#define kTextCtrlMatrix1st2PointY 15
#define kTextCtrlMatrix1st2SizeX 80
#define kTextCtrlMatrix1st2SizeY 30
#define kTextCtrlMatrix1st3PointX 240
#define kTextCtrlMatrix1st3PointY 15
#define kTextCtrlMatrix1st3SizeX 80
#define kTextCtrlMatrix1st3SizeY 30

#define kStaticTextR2Name "R"
#define kStaticTextR2PointX (310 + 30)
#define kStaticTextR2PointY 20
#define kStaticTextR2SizeX 25
#define kStaticTextR2SizeY 25

#define kStaticTextG1Name "G' ="
#define kStaticTextG1PointX 20
#define kStaticTextG1PointY 70
#define kStaticTextG1SizeX 40
#define kStaticTextG1SizeY 25

#define kTextCtrlMatrix2nd1PointX 70
#define kTextCtrlMatrix2nd1PointY 65
#define kTextCtrlMatrix2nd1SizeX 80
#define kTextCtrlMatrix2nd1SizeY 30
#define kTextCtrlMatrix2nd2PointX 155
#define kTextCtrlMatrix2nd2PointY 65
#define kTextCtrlMatrix2nd2SizeX 80
#define kTextCtrlMatrix2nd2SizeY 30
#define kTextCtrlMatrix2nd3PointX 240
#define kTextCtrlMatrix2nd3PointY 65
#define kTextCtrlMatrix2nd3SizeX 80
#define kTextCtrlMatrix2nd3SizeY 30

#define kStaticTextG2Name "G"
#define kStaticTextG2PointX (310 + 30)
#define kStaticTextG2PointY 70
#define kStaticTextG2SizeX 25
#define kStaticTextG2SizeY 25

#define kStaticTextB1Name "B'"
#define kStaticTextB1PointX 20
#define kStaticTextB1PointY 120
#define kStaticTextB1SizeX 25
#define kStaticTextB1SizeY 25

#define kTextCtrlMatrix3rd1PointX 70
#define kTextCtrlMatrix3rd1PointY 115
#define kTextCtrlMatrix3rd1SizeX 80
#define kTextCtrlMatrix3rd1SizeY 30
#define kTextCtrlMatrix3rd2PointX 155
#define kTextCtrlMatrix3rd2PointY 115
#define kTextCtrlMatrix3rd2SizeX 80
#define kTextCtrlMatrix3rd2SizeY 30
#define kTextCtrlMatrix3rd3PointX 240
#define kTextCtrlMatrix3rd3PointY 115
#define kTextCtrlMatrix3rd3SizeX 80
#define kTextCtrlMatrix3rd3SizeY 30

#define kStaticTextB2Name "B"
#define kStaticTextB2PointX (310 + 30)
#define kStaticTextB2PointY 120
#define kStaticTextB2SizeX 25
#define kStaticTextB2SizeY 25

#define kButtonFileDialogName "Open..."
#define kButtonFileDialogPointX 240
#define kButtonFileDialogPointY 160
#define kButtonFileDialogSizeX 80
#define kButtonFileDialogSizeY 30

#define kButtonDefaultName "Default"
#define kButtonDefaultPointX 140
#define kButtonDefaultPointY 160
#define kButtonDefaultSizeX 80
#define kButtonDefaultSizeY 30

#define kButtonApplyName "Apply"
#define kButtonApplyPointX 20
#define kButtonApplyPointY 210
#define kButtonApplySizeX 340
#define kButtonApplySizeY 30

#define kCheckOpenFile  1
#define kCreateOpenFile 2

#define kSpecialValName "*.***"
#define kHideFlagName "HIDE FLAG = "

#define ColorMatrixConfigFile "../lib/Plugins/isp/ColorMatrix.ini"

#endif /* __COLOR_MATRIX_DEFINE_H_*/
