/**
 * @file      template.h
 * @brief     Template plugin of VPF.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

#include "./plugin_base.h"

class TemplateWnd;

/**
 * @class Template
 * @brief Template plugin of VPF.
 */
class Template : public PluginBase {
 private:
  /*! Common parameter */
  CommonParam* common_;
  /*! Setting window */
  TemplateWnd* wnd_;
  /*! Whether initialization has succeeded */
  bool is_success_initialized_;

 public:
  /**
   * @fn
   * Constructor.
   */
  Template(void);

  /**
   * @fn
   * Destructor.
   */
  virtual ~Template(void);

  /**
   * @brief
   * Initialize routine of the Template plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the Template plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the Template plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the Template plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Open setting window of the Template plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);
};
#endif /* _TEMPLATE_H_*/
