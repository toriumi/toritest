/**
 * @file      sensor_define.h
 * @brief     Definition of values for sensor settings plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SENSOR_DEFINE_H_
#define _SENSOR_DEFINE_H_

/* Identification ID of UI.*/
#define kWndId                               90000
#define kStaticTextSensorConfigId            10000
#define kTextCtrlSensorConfigId              10001
#define kButtonFileDialogId                  10002
#define kStaticTextSensorNameId              10003
#define kStaticTextSensorNameValueId         10004
#define kStaticTextImageId                   10005
#define kStaticTextImageSizeValueId          10006
#define kStaticTextBitCountId                10007
#define kComboBoxBitCountId                  10008
#define kButtonSensorSettingsId              10009
#define kButtonApplyId                       10010
#define kStaticTextBinningModeId             10011
#define kButtonApplyFirstPixelId             10012
#define kButtonApplyImageOrientationId       10013
#define kSliderAnalogGainId                  10014
#define kSliderDigitalGainId                 10015
#define kSliderExposureTimeId                10016
#define kStaticTextBinningModeWidthValueId   10017
#define kStaticTextBinningModeHeightValueId  10018
#define kButtonApplyAnalogGainId             10019
#define kButtonApplyDigitalGainId            10020
#define kButtonApplyExposureTimeId           10021

/* Main window definition*/
#define kWndTitle "Sensor config"
#define kWndPointX 30
#define kWndPointY 30
#define kWndSizeW 380
#define kWndSizeH 430

/* Sensor config static text definition*/
#define kStaticTextSensorConfigName "Sensor config file path"
#define kStaticTextSensorConfigPointX 10
#define kStaticTextSensorConfigPointY 10
#define kStaticTextSensorConfigSizeW 300
#define kStaticTextSensorConfigSizeH 30

/* Sensor config text ctrl definition*/
#define kTextCtrlSensorConfigPointX 10
#define kTextCtrlSensorConfigPointY 40
#define kTextCtrlSensorConfigSizeW 300
#define kTextCtrlSensorConfigSizeH 40

/* Sensor config button definition*/
#define kButtonFileDialogName "..."
#define kButtonFileDialogPointX 320
#define kButtonFileDialogPointY 40
#define kButtonFileDialogSizeW 50
#define kButtonFileDialogSizeH 40

/* Sensor name static text definition*/
#define kStaticTextSensorName "Sensor name"
#define kStaticTextSensorNamePointX 10
#define kStaticTextSensorNamePointY 90
#define kStaticTextSensorNameSizeW 200
#define kStaticTextSensorNameSizeH 30

/* Sensor name value static text definition*/
#define kStaticTextSensorNameValue ""
#define kStaticTextSensorNameValuePointX 40
#define kStaticTextSensorNameValuePointY 120
#define kStaticTextSensorNameValueSizeW 200
#define kStaticTextSensorNameValueSizeH 30

/* Image size static text definition*/
#define kStaticTextImageName "Image size"
#define kStaticTextImagePointX 10
#define kStaticTextImagePointY 150
#define kStaticTextImageSizeW 200
#define kStaticTextImageSizeH 30

/* Image size value static text definition*/
#define kStaticTextImageSizeName ""
#define kStaticTextImageSizeValuePointX 40
#define kStaticTextImageSizeValuePointY 180
#define kStaticTextImageSizeValueSizeW 200
#define kStaticTextImageSizeValueSizeH 30

/* Binning mode static text definition*/
#define kStaticTextBinningModeName "Binning mode"
#define kStaticTextBinningModePointX 10
#define kStaticTextBinningModePointY 210
#define kStaticTextBinningModeSizeW 200
#define kStaticTextBinningModeSizeH 30

/* Binning mode width value static text definition*/
#define kStaticTextBinningModeWidthValueName ""
#define kStaticTextBinningModeWidthValuePointX 40
#define kStaticTextBinningModeWidthValuePointY 240
#define kStaticTextBinningModeWidthValueSizeW 200
#define kStaticTextBinningModeWidthValueSizeH 30

/* Binning mode height value static text definition*/
#define kStaticTextBinningModeHeightValueName ""
#define kStaticTextBinningModeHeightValuePointX 40
#define kStaticTextBinningModeHeightValuePointY 270
#define kStaticTextBinningModeHeightValueSizeW 200
#define kStaticTextBinningModeHeightValueSizeH 30

/* Bit count static text definition*/
#define kStaticTextBitCountName "Bit count"
#define kStaticTextBitCountPointX 10
#define kStaticTextBitCountPointY 300
#define kStaticTextBitCountSizeW 200
#define kStaticTextBitCountSizeH 30

/* Bit count combo box definition*/
#define kComboBoxBitCountPointX 10
#define kComboBoxBitCountPointY 330
#define kComboBoxBitCountSizeW 200
#define kComboBoxBitCountSizeH 40

/* Open sensor settings window button definition*/
#define kButtonSensorSettingsName "Sensor settings..."
#define kButtonSensorSettingsPointX 40
#define kButtonSensorSettingsPointY 380
#define kButtonSensorSettingsSizeW 150
#define kButtonSensorSettingsSizeH 40

/* Apply button definition*/
#define kButtonApplyName "Apply"
#define kButtonApplyPointX 280
#define kButtonApplyPointY 380
#define kButtonApplySizeW 80
#define kButtonApplySizeH 40

/* Sensor config open file dialog definition*/
#define kFileDialogSensorConfigOpenWndName "Choose a file to open"
#define kFileDialogSensorConfig "*.xml"
#define kFileDialogSensorSettings "*.csv"

/* Sensor settings window definition*/
#define kWndSettingsTitle "Sensor settings"
#define kWndSettingsPointX 30
#define kWndSettingsPointY 30
#define kWndSettingsSizeW 380
#define kWndSettingsSizeH 380

/* Sensor config static text definition*/
#define kStaticTextSensorSettingsName "Sensor settings file path"
#define kStaticTextSensorSettingsPointX 10
#define kStaticTextSensorSettingsPointY 10
#define kStaticTextSensorSettingsSizeW 300
#define kStaticTextSensorSettingsSizeH 30

/* Analog gain and Digital gain DB param*/
#define kImx219AnalogGainMax 232
//#define kImx219DitalGainMax 256
#define kImx219DitalGainMax 255
#define kIu233AnalogGainMax 224
#define kIu233DitalGainMax 16
#define kImx378AnalogGainMax 978
#define kImx378DitalGainMax 3801
#define kDB "(db)"
#define kMS "(ms)"

#define kBitCountNum 3
#define kBitCount8 "8bit"
#define kBitCount10 "10bit"
#define kBitCount12 "12bit"
#define kProfileClass "ProfileClass"
#define kComment "Comment"
#define kAddress "Address"
#define kAddressValue "Data"
#define kMessageDialogPointX 100
#define kMessageDialogPointY 100
#define kFirstPixelNum 4
#define kFirstPixelR  "R"
#define kFirstPixelGB "GB"
#define kFirstPixelGR "GR"
#define kFirstPixelB  "B"
#define kFirstPixelName "First pixel"
#define kBinningModeUnknown "Unknown"
#define kBinningModeNone    "no-binning"
#define kBinningModeX2      "x2-binning"
#define kBinningModeX4      "x4-binning"
#define kBinningModeAnalog  "analog-binning"

#define kRegisterNone -1
#define kCheckOpenFile  1
#define kCreateOpenFile 2
#define kSensorParamFilePath "../lib/Plugins/input/Sensor.ini"

#define kImx219AnalogGainDBFile \
  "../config/sensor/register_settings/param/IMX219_AnalogGainDB.csv"
#define kImx219DigitalGainDBFile \
  "../config/sensor/register_settings/param/IMX219_DigitalGainDB.csv"
#define kIu233AnalogGainDBFile \
  "../config/sensor/register_settings/param/IU233_AnalogGainDB.csv"
#define kIu233DigitalGainDBFile \
  "../config/sensor/register_settings/param/IU233_DigitalGainDB.csv"
#define kImx378AnalogGainDBFile \
  "../config/sensor/register_settings/param/IMX378_AnalogGainDB.csv"
#define kImx378DigitalGainDBFile \
  "../config/sensor/register_settings/param/IMX378_DigitalGainDB.csv"

#endif /* _SENSOR_DEFINE_H_*/
