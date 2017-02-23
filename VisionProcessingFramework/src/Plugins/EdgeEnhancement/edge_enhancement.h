/**
 * @file      edge_enhancement.h
 * @brief     EdgeEnhancement plugin.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _EDGE_ENHANCEMENT_H_
#define _EDGE_ENHANCEMENT_H_

#include <vector>
#include "./edge_enhancement_define.h"
#include "./edge_enhancement_param.h"
#include "./edge_enhancement_wnd.h"
#include "./include.h"
#include "./plugin_base.h"

class EdgeEnhancementWnd;

/**
 * @class EdgeEnhancement
 * @brief EdgeEnhancement plugin.
 */
class EdgeEnhancement : public PluginBase {
 private:
  /*! Parameter.*/
  EdgeEnhancementParam* param_;

  /*! Parameter setting window.*/
  EdgeEnhancementWnd* wnd_;

  /*! Whether initialization has succeeded */
  bool is_success_initialized_;

 public:
  /**
   * @brief
   * Constructor for this plugin.
   */
  EdgeEnhancement(void);

  /**
   * @brief
   * Destructor for this plugin.
   */
  virtual ~EdgeEnhancement(void);

  /**
   * @brief
   * Initialize routine of the EdgeEnhancement plugin.
   * This function is empty implementation.
   * @param common [in] commom parameters.
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Post-processing routine of the EdgeEnhancement plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the EdgeEnhancement plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  /**
   * @brief
   * Finalize routine of the EdgeEnhancement plugin.
   * This function is empty implementation.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Open setting window of the EdgeEnhancement plugin.
   * @param is_running [in] ImageProcessingState
   */
  virtual void OpenSettingWindow(ImageProcessingState state);

  /**
   * @brief
   * Set the list of parameter setting string for the EdgeEnhancement plugin.
   * @param params [in] settings string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params);
};
#endif /* _EDGE_ENHANCEMENT_H_*/
