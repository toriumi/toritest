/**
 * @file      iplugin.h
 * @brief     Header for IPlugin class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#pragma once
#ifndef _IPLUGIN_H_
#define _IPLUGIN_H_

#include <string>
#include <vector>

#include "./common_param.h"
#include "./include.h"
#include "./port_spec.h"

typedef struct {
  PortSpec* input_port;
  PortSpec* output_port;
} PortRelation;

/*! Interface of Vision Processing Framework plugin */
#define PLUGIN_INTERFACE_VERSION 1

/**
 * @class IPlugin
 * @brief Interface class for Vision Processing Framework plugin.
 */
class IPlugin {
 public:
  /**
   * @brief
   * Constructor.
   */
  IPlugin(void) {}

  /**
   * @brief
   * Destructor.
   */
  virtual ~IPlugin(void) {}

  /**
   * @brief
   * Get a name of the plugin.
   * @return name of the plugin.
   */
  virtual std::string plugin_name(void) = 0;

  /**
   * @brief
   * Get a filepath of the plugin.
   * @return filepath of the plugin.
   */
  virtual std::string file_path(void) = 0;

  /**
   * @brief
   * Set a name of the plugin.
   * @param plugin_name [in] name of the plugin.
   */
  virtual void set_plugin_name(const std::string plugin_name) = 0;

  /**
   * @brief
   * Set a filepath of the plugin.
   * @param file_path [in] filepath of the plugin.
   */
  virtual void set_file_path(const std::string file_path) = 0;

  /**
   * @brief
   * Get a type of the plugin.
   * @return type of the plugin.
   */
  virtual PluginType plugin_type(void) = 0;

  /**
   * @brief
   * Set a type of the plugin.
   * @param plugin_type [in] type of the plugin.
   */
  virtual void set_plugin_type(PluginType plugin_type) = 0;

  /**
   * @brief
   * Add a new plugin to the list of next plugins of this plugin.
   * @param next [in] Pointer to the new plugin.
   */
  virtual void AddNextPlugin(IPlugin* next) = 0;

  /**
   * @brief
   * Get the list of next plugins.
   * @return list of next plugins.
   */
  virtual std::vector<IPlugin*> next_plugins(void) = 0;

  /**
   * @brief
   * Clear the list of next plugins of this plugin.
   */
  virtual void ClearNextPlugins(void) = 0;

  /**
   * @brief
   * Remove a target plugin from the list of next plugins of this plugin.
   * @param plugin_name [in] plugin name of the target plugin.
   */
  virtual void ClearNextPlugins(const std::string plugin_name) = 0;

  /**
   * @brief
   * Set input image size.
   * @param input_size [in] input image size.
   */
  virtual void set_input_image_size(CvSize input_size) = 0;

  /**
   * @brief
   * Get output image size.
   * @return output image size.
   */
  virtual CvSize output_image_size(void) = 0;

  /**
   * @brief
   * Clear the list of next plugins of this plugin.
   * This function removes a connection of each plugin recursively.
   */
  virtual void RemoveNextPlugin(void) = 0;

  /**
   * @brief
   * Remove a target plugin from the list of next plugins of this plugin.
   * This function removes a connection of each plugin recursively.
   * @param plugin_name [in] plugin name of the target plugin.
   */
  virtual void RemoveNextPlugin(const std::string plugin_name) = 0;

  /**
   * @brief
   * Get the list of the intput port.
   * @return list of the input port
   */
  virtual std::vector<PortSpec*> input_port_candidate_specs(void) = 0;

  /**
   * @brief
   * Get a list of the outtput port.
   * @return return list of the output port
   */
  virtual std::vector<PortSpec*> output_port_candidate_specs(void) = 0;

  /**
   * @brief
   * Get the list of port relation.
   * @return list of port relation.
   */
  virtual std::vector<PortRelation*> port_relations(void) = 0;

  /**
   * @brief
   * Get the output port spec that is currently active.
   * @return output port spec.
   */
  virtual PortSpec* output_port_spec(void) = 0;

  /**
   * @brief
   * Set the active output port spec.
   * @param index [in] Index of output port specs.
   */
  virtual void set_active_output_port_spec_index(unsigned int index) = 0;

  /**
   * @brief
   * Get the index of output port specs.
   * @return index of output port specs.
   */
  virtual unsigned int active_output_port_spec_index(void) = 0;

  /**
   * @brief
   * Set a original plugin name to the cloned plugin.
   * And then, clone flag set to  true.
   * @param original_plugin_name [in] original plugin name
   */
  virtual void SetCloneParameter(std::string original_plugin_name) = 0;

  /**
   * @brief
   * Whether the plugin is cloned or not.
   * @return true, cloned plugin.
   */
  virtual bool is_cloned(void) = 0;

  /**
   * @brief
   * Get original plugin name of cloned plugin.
   * @return original plugin name.
   */
  virtual std::string original_plugin_name(void) = 0;

  /**
   * @brief
   * Implement initialize routine of the plugin.
   * @param common [in] pointer of the CommonParam class
   * @return If true, successful initialization
   */
  virtual bool InitProcess(CommonParam* common) = 0;

  /**
   * @brief
   * Implement finalize routine of the plugin.
   */
  virtual void EndProcess(void) = 0;

  /**
   * @brief
   * Implement post-processing routine of the plugin.
   * This function is called by after the main routine per frame.
   */
  virtual void DoPostProcess(void) = 0;

  /**
   * @brief
   * Implement main routine of the plugin.
   * This function is called per frame.
   * @param src_ipl [in] pointer to the src image data.
   * @param dst_ipl [out] pointer to the dst image data.
   * @return If true, success in the main processing
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image) = 0;

  /**
   * @brief
   * Open setting window of the plugin.
   * @param state [in] ImageProcessingState.
   */
  virtual void OpenSettingWindow(ImageProcessingState state) = 0;

  /**
   * @brief
   * Set the image processing state.
   * @param state [in] ImageProcessingState.
   */
  virtual void set_image_processing_state(ImageProcessingState state) = 0;

  /**
   * @brief
   * Get image processing state.
   * @return ImageProcessingState.
   */
  virtual ImageProcessingState image_processing_state(void) = 0;

  /**
   * @brief
   * Whether using destination buffer or not.
   * @return true, use destination buffer
   */
  virtual bool is_use_dest_buffer(void) = 0;

  /**
   * @brief
   * Get the plugin interface version
   * @return plugin interface version.
   */
  virtual unsigned int plugin_interface_version(void) = 0;

  /**
   * @brief
   * Set the logger function.
   * @param logger_func [in] pointer of log output function.
   */
  virtual void set_logger_func(void* logger_func) = 0;

  /**
   * @brief
   * Set output image size.
   * @param output_size [in] output image size.
   */
  virtual void set_output_image_size(CvSize output_size) = 0;

  /**
   * @brief
   * Change the active index of output port spec candidates.
   * @param index [in] active index of output port spec.
   * @return true, success to change of the output port
   */
  virtual bool ChangeOutputPortSpec(unsigned int index) = 0;

  /**
   * @brief
   * Set the processing time(DoProcess).
   * Calculate the average processing time for 10 times average.
   * @param time [in] processing time
   */
  virtual void set_proc_time(float time) = 0;

  /**
   * @brief
   * Get the most recently measured processing time.
   * @return processing time for 10 times average.
   */
  virtual float proc_time(void) = 0;

  /**
   * @brief
   * Get the list of parameter setting string defined by each plugin to save the
   * parameter of the plugin.
   * @return list of string.
   */
  virtual std::vector<wxString> GetPluginSettings(void) = 0;

  /**
   * @brief
   * Set the list of parameter setting string defined by each plugin to save the
   * parameter of the plugin.
   * @param params [in] list of string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params) = 0;

  /**
   * @brief
   * Clear the list of parameter setting string.
   * @param [out] parameter values
   */
  virtual void ClearPluginSettings(void) = 0;

  /**
   * @brief
   * Add a string to the list of parameter setting string.
   * @param [in] string with the parameter values.
   */
  virtual void AddLinePluginSettings(wxString params) = 0;

 protected:
  /**
   * @brief
   * Add the input candidate port spec.
   * @param type [in] PlaneType
   * @return input port candidate spec id
   */
  virtual int AddInputPortCandidateSpec(PlaneType type) = 0;

  /**
   * @brief
   * Add the output candidate port spec.
   * @param type [in] PlaneType
   * @return output port candidate spec id
   */
  virtual int AddOutputPortCandidateSpec(PlaneType type) = 0;

  /**
   * @brief
   * Add the port relation by input/output port candidate spec id
   * @param input_index [in] input port candidate spec id
   * @param output_index [in] output port candidate spec id
   * @return true, success in additional port relation
   */
  virtual bool AddPortRelation(unsigned int input_index,
                               unsigned int output_index) = 0;

  /**
   * @brief
   * Set whether to use the dest buffer in output.
   * @param is_use_dest_buffer [in] if true, use dest buffer
   */
  virtual void set_is_use_dest_buffer(bool is_use) = 0;
};

#endif /* _IPLUGIN_H_ */
