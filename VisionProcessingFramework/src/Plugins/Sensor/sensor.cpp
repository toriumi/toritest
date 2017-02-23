/**
 * @file      sensor.cpp
 * @brief     Sensor plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./sensor.h"
#include <vector>
#include "./sensor_settings_wnd.h"
#include "./sensor_wnd.h"

#include <unistd.h>

static wxMutex mutex_finalize;
Sensor *gloval_sensor_;

extern "C" {
  int __CCIRegRead(struct ssp_handle *handle, int address, int *data);
  int __CCIRegWrite(struct ssp_handle *handle, int address, int data);
  int __CCIRegReadBySlaveAddress(int CCISlaveAddress, int RegAddress, int *data);
  int __CCIRegWriteBySlaveAddress(int CCISlaveAddress, int RegAddress, int data);
}

/**
 * @brief
 * Constructor.
 */
Sensor::Sensor() : PluginBase() {
  DEBUG_PRINT("Sensor::Sensor()\n");
  /* Initialize*/
  common_ = NULL;
  finalize_on_ = false;
  gloval_sensor_ = this;
  frame_buffer_ = NULL;
  temp_frame_buffer_ = NULL;
  last_image_ = NULL;
  ssp_handle_ = NULL;
  ssp_profile_ = NULL;
  start_streaming_ = false;

  first_pixel_ = 0;
  analog_gain_param_ = kRegisterNone;
  digital_gain_param_ = kRegisterNone;
  coarse_integration_time_ = kRegisterNone;
  orien_reg_ = kRegisterNone;
  analog_gain_param_temp_ = kRegisterNone;
  digital_gain_param_temp_ = kRegisterNone;
  coarse_integration_time_temp_ = kRegisterNone;
  orien_reg_temp_ = kRegisterNone;

  sensor_on_init_ = false;
  buffer_lock_ = new wxMutex(wxMUTEX_DEFAULT);
  callback_wait_sem_ = new wxSemaphore(1, 3);

  sensor_config_file_path_ = NULL;

  set_plugin_name("Sensor");

  // Create output port.
  AddOutputPortCandidateSpec(kGRAY8);
  AddOutputPortCandidateSpec(kGRAY16);

  /* Set output image size.*/
  CvSize size = cvSize(1280, 720);
  set_output_image_size(size);

  /* Set output port number and activate output port.*/
  set_active_output_port_spec_index(0);
  std::vector<PortSpec *> out_ports = output_port_candidate_specs();
  PortSpec *out_port_spec = out_ports[0];
  out_port_spec->set_available(true);

  set_is_use_dest_buffer(true);

  sensor_wnd_ = new SensorWnd(this);
  sensor_settings_wnd_ = new SensorSettingsWnd(this);
}

/**
 * @brief
 * Destructor.
 */
Sensor::~Sensor() {
  Finalize();
  delete sensor_wnd_;
  delete sensor_settings_wnd_;
}

/**
 * @brief
 * Make the initial process of the sensor to the SSP.
 * @param bit_count_type [in] bit count.
 * @param sensor_config_file_path [in] sensor config file path.
 * @param size [in] image size.
 * @return If true, sensor initialization process is successful.
 */
bool Sensor::SensorConfig(int bit_count_type,
                          char *sensor_config_file_path, CvSize size) {
  DEBUG_PRINT("Sensor::SensorConfig \n");
  int retval;

  if (sensor_on_init_ == true) {
    if (Finalize() == false) {
      DEBUG_PRINT("Failed to sensor finalize \n");
    }
  }
  sensor_config_file_path_ = sensor_config_file_path;

  /* Read the sensor file to the SSP.*/
  if ((retval = ssp_read_profile(sensor_config_file_path,
                                 &ssp_profile_)) != SSP_SUCCESS) {
    return false;
  }

  //ssp_settings_.lib_settings.NumFrameFIFOSize = 5;
  ssp_settings_.lib_settings.NumFrameFIFOSize = 2;
  ssp_settings_.lib_settings.NumThreadForBuildInPreprocess = 1;
  ssp_settings_.lib_settings.frame_drop_cam_user_func = frame_drop_preprocess;
  ssp_settings_.lib_settings.frame_drop_pre_user_func = frame_drop;
  ssp_settings_.lib_settings.fame_preprocess_user_func = frame_preprocess;
  ssp_settings_.frame_preprocess_options.FormatConvertType = bit_count_type;
  ssp_settings_.camera_settings.PowerOnResetUsec = 1000000;

  /* Make the initial process of the sensor to the SSP.*/
  if ((retval = ssp_initialize(&ssp_handle_, ssp_profile_, &ssp_settings_)) !=
      SSP_SUCCESS) {
    DEBUG_PRINT("initialize failure = %d\n", retval);
    return false;
  }
  if((retval = ssp_stop_streaming(ssp_handle_)) != SSP_SUCCESS){
    DEBUG_PRINT("Can not stop streaming. %x\n",retval);
  }

  /*To set the size*/
  set_output_image_size(size);
  size_ = size;

  /*To set the bit count*/
  bit_count_type_ = bit_count_type;
  if (bit_count_type == 0x02) {
    set_active_output_port_spec_index(0);
  } else {
    set_active_output_port_spec_index(1);
  }

  sensor_on_init_ = true;

  /*To set the frequency*/
  frequency_ = ssp_profile_->ImageProperty.Frequency;

  int etime_regdata1 = 0x00;
  int etime_regdata2 = 0x00;
  if (__CCIRegRead(ssp_handle_, etime_addr1_, &etime_regdata1) == 0) {
	  DEBUG_PRINT("Coarse Integration Time Reading register error.\n");
  }
  if (__CCIRegRead(ssp_handle_, etime_addr2_, &etime_regdata2) == 0) {
	  DEBUG_PRINT("Coarse Integration Time Reading register error.\n");
  }
  coarse_regvalue_ = (etime_regdata1 << 8) + etime_regdata2;

  DEBUG_PRINT("initialize success\n");

  int line_length_regdata1 = 0x00;
  int line_length_regdata2 = 0x00;
  int line_length_regadd2191 = 0x0160;
  int line_length_regadd2192 = 0x0161;
  int line_length_regadd2331 = 0x0340;
  int line_length_regadd2332 = 0x0341;
  if (this->sensor_type_ == wxT("IMX219")){
    if (__CCIRegRead(ssp_handle_, line_length_regadd2191, &line_length_regdata1) == 0) {
	  DEBUG_PRINT("Frame Line Length Reading register error.\n");
    }
    if (__CCIRegRead(ssp_handle_, line_length_regadd2192, &line_length_regdata2) == 0) {
  	  DEBUG_PRINT("Frame Line Length Reading register error.\n");
    }
    line_length_regvalue_ = (line_length_regdata1 << 8) + line_length_regdata2;
  }
  if ((this->sensor_type_ == wxT("IMX408")) ||
      (this->sensor_type_ == wxT("IU233")) ||
      (this->sensor_type_ == wxT("IMX378"))) {
    if (__CCIRegRead(ssp_handle_, line_length_regadd2331, &line_length_regdata1) == 0) {
  	  DEBUG_PRINT("Frame Line Length Reading register error.\n");
    }
    if (__CCIRegRead(ssp_handle_, line_length_regadd2332, &line_length_regdata2) == 0) {
  	  DEBUG_PRINT("Frame Line Length Reading register error.\n");
    }
    line_length_regvalue_ = (line_length_regdata1 << 8) + line_length_regdata2;
  }

  return true;
}

/**
 * @brief
 * To set a common the first pixel.
 * @param first_pixel [in] first pixel.
 */
void Sensor::SetFirstPixel(int first_pixel) {
  first_pixel_ = first_pixel;
  if (common_ != NULL) {
    common_->set_first_pixel(first_pixel_);
    DEBUG_PRINT("common_first:%d \n", first_pixel_);
  }
}

/**
 * @brief
 * To set the binning mode.
 */
void Sensor::SetBinningMode() {
  DEBUG_PRINT("Sensor::SetBinningMode \n");
  if (sensor_wnd_->SetBinningMode() == false) {
    DEBUG_PRINT("Failed to set binning mode \n");
  }
}

/**
 * @brief
 * Initialize routine of the Sensor plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization.
 */
bool Sensor::InitProcess(CommonParam *common) {
  DEBUG_PRINT("Sensor::InitProcess \n");
  int retval;
  int optical_black;

  finalize_on_ = false;
  common_ = common;
  common_->set_first_pixel(first_pixel_);

  if (sensor_on_init_ == false) {
    DEBUG_PRINT("Sensor none init. please on apply \n");
    return false;
  }

  /* To start the streaming of the sensor to the SSP.(IMX378) */
  if (this->sensor_type_ == wxT("IMX378")) {
    if((retval = ssp_start_streaming(ssp_handle_)) != SSP_SUCCESS){
    	DEBUG_PRINT("Can not start streaming. %x\n",retval);
      return false;
    }
    usleep(1000);
    ssp_flush_event(ssp_handle_);
    if((retval = ssp_stop_streaming(ssp_handle_)) != SSP_SUCCESS){
    	DEBUG_PRINT("Can not stop streaming. %x\n",retval);
      return false;
    }
  }

  /* To start the streaming of the sensor to the SSP.*/
  if ((retval = ssp_start_streaming(ssp_handle_)) != SSP_SUCCESS) {
    DEBUG_PRINT("Can not start streaming. %x\n", retval);
    return false;
  }
  start_streaming_ = true;
  if (bit_count_type_ == 0x02) {
    optical_black = 16;
    common_->set_optical_black(optical_black);
    frame_buffer_ = new cv::Mat(size_, CV_8UC1);
    temp_frame_buffer_ = new cv::Mat(size_, CV_8UC1);
    last_image_ = new cv::Mat(size_, CV_8UC1);
  } else {
    optical_black = 64;
    common_->set_optical_black(optical_black);
    frame_buffer_ = new cv::Mat(size_, CV_16UC1);
    temp_frame_buffer_ = new cv::Mat(size_, CV_16UC1);
    last_image_ = new cv::Mat(size_, CV_16UC1);
  }
  return true;
}

/**
 * @brief
 * Finalize routine of the Sensor plugin.
 * This function is empty implementation.
 */
void Sensor::EndProcess() {
  DEBUG_PRINT("Sensor::EndProcess \n");
  finalize_on_ = true;
  wxMutexLocker lock(mutex_finalize);

  if (StopStreaming() == false) {
    DEBUG_PRINT("Failed to sensor finalize \n");
  }
  DEBUG_PRINT("Sensor::EndProcess success \n");
  return;
}

/**
 * @brief
 * Post-processing routine of the Sensor plugin.
 * This function is empty implementation.
 */
void Sensor::DoPostProcess(void) {
  DEBUG_PRINT("Sensor::DoPostProcess \n");

  /* To reflect the register of the analog gain.*/
  if (analog_gain_param_ != kRegisterNone) {
    if (this->sensor_type_ == wxT("IMX378")) {
      int RegParam = 0;
      ushort Reg = (ushort)analog_gain_param_;
      RegParam = Reg >> 8;
      if (__CCIRegWrite(ssp_handle_, again_addr_, RegParam) == 0) {
        DEBUG_PRINT("Analog Gain Writing register error.\n");
      }
      RegParam = Reg & 0x00FF;
      if (__CCIRegWrite(ssp_handle_, again_addr1_, RegParam) == 0) {
        DEBUG_PRINT("Analog Gain Writing register error1.\n");
      }
      analog_gain_param_ = kRegisterNone;
    } else {
      if (__CCIRegWrite(ssp_handle_, again_addr_, analog_gain_param_) == 0) {
        DEBUG_PRINT("Analog Gain Writing register error.\n");
      }
      analog_gain_param_ = kRegisterNone;
    }
  }

  /* To reflect the register of the digital gain.*/
  if (digital_gain_param_ != kRegisterNone) {
    if (this->sensor_type_ == wxT("IMX378")) {
      int RegParam = 0;
      ushort Reg = (ushort)digital_gain_param_;
      RegParam = Reg >> 8;
      if (__CCIRegWrite(ssp_handle_, dgain_addr1_, RegParam) == 0) {
        DEBUG_PRINT("Digital Gain Writing register error.\n");
      }
      RegParam = Reg & 0x00FF;
      if (__CCIRegWrite(ssp_handle_, dgain_addr2_, RegParam) == 0) {
        DEBUG_PRINT("Digital Gain Writing register error.1.\n");
      }
      digital_gain_param_ = kRegisterNone;
    } else {
      std::vector<int>::iterator itr;
      for (itr = dgain_addr_.begin(); itr != dgain_addr_.end(); itr++) {
        if (__CCIRegWrite(ssp_handle_, *itr, digital_gain_param_) == 0) {
          DEBUG_PRINT("Digital Gain Writing register error.\n");
        }
      }
      digital_gain_param_ = kRegisterNone;
	}
  }

  /* To reflect the register of the coarse integration time.*/
  if (coarse_integration_time_ != kRegisterNone) {
    int RegParam = 0;
    ushort Reg = (ushort)coarse_integration_time_;

    RegParam = Reg >> 8;

    if (__CCIRegWrite(ssp_handle_, etime_addr1_, RegParam) == 0) {
      DEBUG_PRINT("Coarse Integration Time Writing register error1.\n");
    }

    RegParam = Reg & 0x00FF;

    if (__CCIRegWrite(ssp_handle_, etime_addr2_, RegParam) == 0) {
      DEBUG_PRINT("Coarse Integration Time Writing register error2.\n");
    }
    coarse_integration_time_ = kRegisterNone;
  }

  /* To reflect the register of the image orientation.*/
  if (orien_reg_ != kRegisterNone) {
    if (__CCIRegWrite(ssp_handle_, image_addr_, orien_reg_) == 0) {
      DEBUG_PRINT("image orientation register error1.\n");
    }
    orien_reg_ = kRegisterNone;
  }
  return;
}

/**
 * @brief
 * Main routine of the Sensor plugin.
 * @param src_image [in] src image data.
 * @param dst_image [out] dst image data.
 * @return If true, success in the main processing.
 */
bool Sensor::DoProcess(cv::Mat *src_image, cv::Mat *dst_image) {
  DEBUG_PRINT("Sensor::DoProcess \n");
  /* Wait for a call back from the SSP.*/
  wxSemaError retval;
  if ((retval = callback_wait_sem_->WaitTimeout(10000)) == wxSEMA_TIMEOUT) {
    *dst_image = *last_image_;
  } else {
    buffer_lock_->Lock();
    /* Frame data copy.*/
    *last_image_ = frame_buffer_->clone();
    *dst_image = frame_buffer_->clone();
    buffer_lock_->Unlock();
  }
  return true;
}

/**
 * @brief
 * Open sensor config window of the Sensor plugin.
 * @param state [in] ImageProcessingState
 */
void Sensor::OpenSettingWindow(ImageProcessingState state) {
  if (sensor_wnd_ == NULL) {
    DEBUG_PRINT("sensor_wnd_ == NULL\n");
    return;
  }
  wxString window_title(plugin_name().c_str(), wxConvUTF8);
  sensor_wnd_->SetTitle(window_title);
  sensor_wnd_->InitDialog();
  sensor_wnd_->Show(true);
}

/**
 * @brief
 * Close sensor config window of the Sensor plugin.
 * @param state [in] ImageProcessingState
 */
void Sensor::CloseSettingWindow(ImageProcessingState state) {
  if (sensor_wnd_ == NULL) {
    DEBUG_PRINT("sensor_wnd_ == NULL\n");
    return;
  }
  sensor_wnd_->Show(false);
}

/**
 * @brief
 * Open sensor setting window of the Sensor plugin.
 */
bool Sensor::OpenSensorSettingWindow() {
  if (sensor_settings_wnd_ == NULL) {
    DEBUG_PRINT("sensor_settings_wnd_ == NULL\n");
    return false;
  }
  sensor_settings_wnd_->Show(true);
  return true;
}

/**
 * @brief
 * Initialization of the sensor settings window.
 */
bool Sensor::InitSensorSettingWindow() {
  if (sensor_settings_wnd_ == NULL) {
    DEBUG_PRINT("sensor_settings_wnd_ == NULL\n");
    return false;
  }
  delete sensor_settings_wnd_;
  sensor_wnd_->WriteSensorParamtoFile(wxT(kSensorParamFilePath),
                                      wxT(""),
                                      8);
  sensor_settings_wnd_ = new SensorSettingsWnd(this);
  return true;
}

/**
 * @brief
 * Close setting window of plugin.
 */
bool Sensor::CloseSettingWindow() {
  if (sensor_wnd_ == NULL) {
    return false;
  }
  sensor_wnd_->Show(false);
  return true;
}

/**
 * @brief
 * Set the image processing state.
 * @param state [in] ImageProcessingState
 */
void Sensor::set_image_processing_state(ImageProcessingState state) {
  DEBUG_PRINT("Sensor::set_image_processing_state() state = %d\n", state);
  switch (state) {
    case kRun:
      break;
    case kStop:
      StopStreaming();
      break;
    case kPause:
      break;
  }
  sensor_wnd_->UpdateUIForImageProcessingState(state);
}

/**
 * @brief
 * Make the termination process of the sensor to the SSP.
 * @return If true, the success of the SSP end processing.
 */
bool Sensor::Finalize() {
  int retval;
  sensor_on_init_ = false;
  frame_count_ = 0;

  if (ssp_handle_ != NULL) {
    if (start_streaming_ == true) {
      start_streaming_ = false;
      /* To stop the streaming of the sensor to the SSP.*/
      if ((retval = ssp_stop_streaming(ssp_handle_)) != SSP_SUCCESS) {
        return false;
      }
      /*To release the sensor handle to SSP.*/
      ssp_flush_event(ssp_handle_);
    }
    /*Makes the sensor end processing to SSP.*/
    if (ssp_finalize(ssp_handle_) != SSP_SUCCESS) {
      return false;
    }
    ssp_handle_ = NULL;
    ssp_profile_ = NULL;
  }

  /* Delete image buffer*/
  if (frame_buffer_ != NULL) {
    delete frame_buffer_;
    frame_buffer_ = NULL;
  }
  if (temp_frame_buffer_ != NULL) {
    delete temp_frame_buffer_;
    temp_frame_buffer_ = NULL;
  }
  if (last_image_ != NULL) {
    delete last_image_;
    last_image_ = NULL;
  }
  return true;
}

/**
 * @brief
 * To stop the streaming of the sensor to the SSP.
 * @return If true, the success of streaming stop processing of the SSP.
 */
bool Sensor::StopStreaming() {
  int retval;
  frame_count_ = 0;

  if (ssp_handle_ != NULL) {
    if (start_streaming_ == true) {
      start_streaming_ = false;
      /* To stop the streaming of the sensor to the SSP.*/
      if ((retval = ssp_stop_streaming(ssp_handle_)) != SSP_SUCCESS) {
        return false;
      }
      /*To release the sensor handle to SSP.*/
      ssp_flush_event(ssp_handle_);
    }
  }

  // delete image buffer
  if (frame_buffer_ != NULL) {
    delete frame_buffer_;
    frame_buffer_ = NULL;
  }
  if (temp_frame_buffer_ != NULL) {
    delete temp_frame_buffer_;
    temp_frame_buffer_ = NULL;
  }
  if (last_image_ != NULL) {
    delete last_image_;
    last_image_ = NULL;
  }
  return true;
}

/**
 * @brief
 * Callback function for the acquisition SSP of the frame.
 * @param handle [in] ssp handle
 * @param frame [in] ssp frame
 */
void Sensor::frame_preprocess(struct ssp_handle *handle,
                              struct ssp_frame *frame) {
  DEBUG_PRINT("Sensor::frame_preprocess start \n");

  if (static_cast<Sensor *>(gloval_sensor_)->finalize_on_ == true) {
    DEBUG_PRINT("Sensor::finalize_on \n");
    return;
  }
  wxMutexLocker lock(mutex_finalize);

//  if (static_cast<Sensor *>(gloval_sensor_)->frame_count_ <= 5) {
    /*Release frame*/
//    ssp_release_frame(frame);
  static_cast<Sensor *>(gloval_sensor_)->frame_count_++;
//    return;
//  }
//  DEBUG_PRINT("Sensor::frame_preprocess frame shori\n");
  if (static_cast<Sensor *>(gloval_sensor_)->start_streaming_ == false) {
	 DEBUG_PRINT("Sensor::frame_preprocess start_streaming_ == false\n");
     return;
  }
  if (static_cast<Sensor *>(gloval_sensor_)->temp_frame_buffer_ == NULL) {
	DEBUG_PRINT("Sensor::temp_frame_buffer_ == NULL\n");
    return;
  }

  static_cast<Sensor *>(gloval_sensor_)->buffer_lock_->Lock();

  /*Get frame*/
  unsigned char *frame_data = ssp_get_frame_data(frame);
  int frame_size = 0;
  /*Get frame size*/
  ssp_get_frame_size(frame, &frame_size);//
  // frame data copy
  memcpy(static_cast<Sensor *>(gloval_sensor_)->temp_frame_buffer_->data,
         frame_data, frame_size);

  /*Release frame*/
  ssp_release_frame(frame);

  /*Frame copy*/
  cv::Mat *temp = static_cast<Sensor *>(gloval_sensor_)->temp_frame_buffer_;
  static_cast<Sensor *>(gloval_sensor_)->temp_frame_buffer_ =
      static_cast<Sensor *>(gloval_sensor_)->frame_buffer_;
  static_cast<Sensor *>(gloval_sensor_)->frame_buffer_ = temp;

  static_cast<Sensor *>(gloval_sensor_)->buffer_lock_->Unlock();

  // sem_post
  static_cast<Sensor *>(gloval_sensor_)->callback_wait_sem_->Post();
  static_cast<Sensor *>(gloval_sensor_)->frame_count_ = 0;
  DEBUG_PRINT("Sensor::frame_preprocess success \n");
}

/**
 * @brief
 * When the FIFO is SSP during the FULL attempts to write the frame to the 
 * FIFO, or, callback function to be called in the event of a failed memory 
 * allocation
 * @param handle [in] ssp handle
 */
void Sensor::frame_drop(struct ssp_handle *handle) {
  DEBUG_PRINT("Sensor::frame_drop \n");
}

/**
 * @brief
 * The callback function to be called when the memory allocation fails during
 * the frame image preprocessing.
 * @param handle [in] ssp handle
 */
void Sensor::frame_drop_preprocess(struct ssp_handle *handle) {
  DEBUG_PRINT("Sensor::frame_drop_preprocess \n");
}

/**
 * @brief
 * Set the list of parameter setting string for the Sensor plugin.
 * @param params [in] settings string.
 */
void Sensor::SetPluginSettings(std::vector<wxString> params) {
  sensor_wnd_->SetSensorConfig(params);
  sensor_settings_wnd_->SetSensorSettings(params);
}

extern "C" PluginBase *Create(void) {
  DEBUG_PRINT("Create Template plugins\n");
  Sensor *plugin = new Sensor();
  return plugin;
}

extern "C" void Destroy(PluginBase *plugin) { delete plugin; }
