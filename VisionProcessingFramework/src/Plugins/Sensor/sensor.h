/**
 * @file      sensor.h
 * @brief     Sensor plugin header.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <wiringPi.h>
#include <vector>
#include <sys/time.h>
#include "./plugin_base.h"
#include "./sensor_define.h"

extern "C" {
  #include "./include/libssp.h"
}

class SensorWnd;
class SensorSettingsWnd;

/**
 * @class Sensor
 * @brief Using the SPP, obtains a frame data from the sensor.
 */
class Sensor : public PluginBase {
 private:
  /*! Sensor window class object.*/
  SensorWnd *sensor_wnd_;

  /*! Sensor settings window class object.*/
  SensorSettingsWnd *sensor_settings_wnd_;

  /*! Common parameter.*/
  CommonParam *common_;

  /*! first pixel.*/
  int first_pixel_;

  /*! Frame buffer.*/
  cv::Mat *frame_buffer_;

  /*! Temporary frame buffer.*/
  cv::Mat *temp_frame_buffer_;

  /*! Last Frame buffer.*/
  cv::Mat *last_image_;

  /*! SSP sensor handle.*/
  struct ssp_handle *ssp_handle_;

  /*! SSP sensor profile.*/
  struct ssp_profile *ssp_profile_;

  /*! SSP sensor settings param.*/
  struct ssp_settings ssp_settings_;

  /*! Frame buffer mutex.*/
  wxMutex *buffer_lock_;

  /*! Semaphore to wait for a call back.*/
  wxSemaphore *callback_wait_sem_;

  /*! Sensor config file path.*/
  //char *sensor_config_file_path_;

  /*! Image size.*/
  CvSize size_;

  /*! Bit count.*/
  int bit_count_type_;

  /*! SSP initialize flag.*/
  bool sensor_on_init_;

  /*! SSP start streaming flag.*/
  bool start_streaming_;

  /*! SSP end processing flag.*/
  bool finalize_on_;

 public:
  /*! Sensor config file path.*/
  char *sensor_config_file_path_;

  /*! Sensor config type.*/
  wxString sensor_config_type_;

  /*! Sensor type.*/
  wxString sensor_type_;

  /*! Sensor settings file path.*/
  wxString sensor_settings_file_path_;

  /*! Image orientation register address.*/
  int image_addr_;

  /*! Analog gain register address.*/
  int again_addr_;

  /*! Analog gain register address1.*/
  int again_addr1_;

  /*! Digital gain register address.*/
  std::vector<int> dgain_addr_;

  /*! Digital gain register address1.(IMX378)*/
  int dgain_addr1_;

  /*! Digital gain register address2.(IMX378)*/
  int dgain_addr2_;

  /*! Coarse integration time register address1.*/
  int etime_addr1_;

  /*! Coarse integration time register address2.*/
  int etime_addr2_;

  /*! Binning mode register address.*/
  std::vector<int> binning_mode_address_;

  /*! Binning mode register value.*/
  std::vector<int> binning_mode_value_;

  /*! Analog gain register value.*/
  int analog_gain_param_;

  /*! Digital gain register value.*/
  int digital_gain_param_;

  /*! Coarse integration time register value.*/
  int coarse_integration_time_;

  /*! Image orientation register value.*/
  int orien_reg_;

  /*! Temporary analog gain register value.*/
  int analog_gain_param_temp_;

  /*! Temporary digital gain register value.*/
  int digital_gain_param_temp_;

  /*! Temporary coarse integration time register value.*/
  int coarse_integration_time_temp_;

  /*! Temporary image orientation register value.*/
  int orien_reg_temp_;

  /*! Frame count.*/
  int frame_count_;

  /*! Frame length line.*/
  int frame_length_line_;

  /*! Offset.*/
  int offset_;

  /*! Frequency.*/
  int frequency_;

  /*! Coarse Integration Time.*/
  float coarse_regvalue_;

  /*! Frame Line Length.*/
  int line_length_regvalue_;

  /**
   * @brief
   * Constructor.
   */
  Sensor(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~Sensor(void);

  /**
   * @brief
   * Initialize routine of the Sensor plugin.
   * @param bit_count_type [in] bit count.
   * @param sensor_config_file_path [in] sensor config file path.
   * @param size [in] image size.
   * @return If true, sensor initialization process is successful.
   */
  virtual bool SensorConfig(int bit_count_type,
                            char *sensor_config_file_path, CvSize size);

  /**
   * @brief
   * To set a common the first pixel.
   * @param first_pixel [in] first pixel.
   */
  virtual void SetFirstPixel(int first_pixel);

  /**
   * @brief
   * To get the first pixel.
   */
  virtual int FirstPixel() { return first_pixel_; }

  /**
   * @brief
   * To set a common the binning mode.
   */
  virtual void SetBinningMode();

  /**
   * @brief
   * Initialize routine of the Sensor plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam *common);

  /**
   * @brief
   * End process for plugin.
   */
  /*  virtual bool EndProcess(void); */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the Sensor plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the Bin plugin.
   * @param src_image [in] src image data.
   * @param dst_image [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat *src_image, cv::Mat *dst_image);

  /**
   * @brief
   * Open setting window of plugin.
   * @param state [in] Image processing state
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Close setting window of plugin.
   * @param state [in] Image processing state
   */
  virtual void CloseSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Open sensor setting window of the Sensor plugin.
   */
  virtual bool OpenSensorSettingWindow();

  /**
   * @brief
   * Initialization of the sensor settings window.
   */
  virtual bool InitSensorSettingWindow();

  /**
   * @brief
   * Set image processing state.
   * @param state [in] image processing state.
   */
  virtual void set_image_processing_state(ImageProcessingState state);

  /**
   * @brief
   * Close setting window of plugin.
   * @return success or false.
   */
  virtual bool CloseSettingWindow(void);

  /**
   * @brief
   * Make the termination process of the sensor to the SSP.
   * @return If true, the success of the SSP end processing.
   */
  virtual bool Finalize(void);

  /**
   * @brief
   * To stop the streaming of the sensor to the SSP.
   * @return If true, the success of streaming stop processing of the SSP.
   */
  virtual bool StopStreaming(void);

  /**
   * @brief
   * Callback function for the acquisition SSP of the frame.
   * @param handle [in] ssp handle
   * @param frame [in] ssp frame
   */
  static void frame_preprocess(struct ssp_handle *handle,
                               struct ssp_frame *frame);

  /**
   * @brief
   * When the FIFO is SSP during the FULL attempts to write the frame to the 
   * FIFO, or, callback function to be called in the event of a failed memory 
   * allocation
   * @param handle [in] ssp handle
   */
  static void frame_drop(struct ssp_handle *handle);

  /**
   * @brief
   * The callback function to be called when the memory allocation fails during
   * the frame image preprocessing.
   * @param handle [in] ssp handle
   */
  static void frame_drop_preprocess(struct ssp_handle *handle);

  /**
   * @brief
   * Set the list of parameter setting string for the Sensor plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);
};
#endif /* _SENSOR_H_*/
