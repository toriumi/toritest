/**
 * @file      plugin_base.h
 * @brief     Header for PluginBase class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _PLUGIN_BASE_
#define _PLUGIN_BASE_

#include <string>
#include <vector>

#include "./common_param.h"
#include "./include.h"
#include "./iplugin.h"
#include "./log_level.h"
#include "./port_spec.h"

/**
 * @class PluginBase
 * @brief Base class for Vision Processing Framework plugin.
 */
class PluginBase : public IPlugin {
 private:
  /*! Name of the plugin */
  std::string plugin_name_;

  /*! FilePath of the plugin */
  std::string file_path_;

  /*! Type of the plugin */
  PluginType plugin_type_;

  /*! Input image size which the previous plugin notifies to this plugin */
  CvSize input_image_size_;

  /*! Output image size which this plugin notifies to next plugin */
  CvSize output_image_size_;

  /*! List of a next plugin. */
  std::vector<IPlugin*> next_plugins_;

  /*! List of a candidate speciification for input port. */
  std::vector<PortSpec*> input_port_candidate_specs_; /* port_spec.h */

  /*! List of a candidate speciification for output port. */
  std::vector<PortSpec*> output_port_candidate_specs_; /* port_spec.h */

  /*! List of a input-output port relation */
  std::vector<PortRelation*> port_relations_;

  /*! Index of the valid output port specs list. */
  unsigned int active_output_port_spec_index_;

  /*! Is use dest buffer. */
  bool is_use_dest_;

  /*! Cloned Plugin flag. */
  bool is_cloned_;

  /*! Original name of the cloned plugin. */
  std::string original_plugin_name_;

  /* Image processing state. */
  ImageProcessingState image_processing_state_;

  /*! counter for processing time measurement */
  int proc_time_counter_;

  /*! processing time (10times average) */
  float proc_time_;

  /*! total processing time (10times) */
  float sum_proc_time_;

  /*! used for the list of parameter setting string */
  std::vector<wxString> setting_params_;

 protected:
  /*! Logger function */
  void* logger_func_;

 public:
  /**
   * @brief
   * Constructor.
   */
  PluginBase(void) {
    plugin_name_ = "Unknown";
    file_path_ = "";
    input_image_size_ = cvSize(0, 0);
    output_image_size_ = cvSize(0, 0);
    active_output_port_spec_index_ = 0;
    is_use_dest_ = true;
    logger_func_ = NULL;
    is_cloned_ = false;
    original_plugin_name_ = "";
    proc_time_counter_ = 0;
    proc_time_ = 0.0f;
  }

  /**
   * @brief
   * Destructor.
   */
  virtual ~PluginBase(void) {
    for (unsigned int i = 0; i < input_port_candidate_specs_.size(); i++) {
      delete input_port_candidate_specs_[i];
    }
    for (unsigned int i = 0; i < output_port_candidate_specs_.size(); i++) {
      delete output_port_candidate_specs_[i];
    }
    for (unsigned int i = 0; i < port_relations_.size(); i++) {
      delete port_relations_[i];
    }
  }

  /**
   * @brief
   * Get a name of the plugin.
   * @return name of the plugin.
   */
  std::string plugin_name(void) { return plugin_name_; }

  /**
   * @brief
   * Get a filepath of the plugin.
   * @return filepath of the plugin.
   */
  std::string file_path(void) { return file_path_; }

  /**
   * @brief
   * Set a name of the plugin.
   * @param plugin_name [in] name of the plugin.
   */
  void set_plugin_name(const std::string plugin_name) {
    plugin_name_ = plugin_name;
  }

  /**
   * @brief
   * Set a filepath of the plugin.
   * @param file_path [in] filepath of the plugin.
   */
  void set_file_path(const std::string file_path) { file_path_ = file_path; }

  /**
   * @brief
   * Get a type of the plugin.
   * @return type of the plugin.
   */
  PluginType plugin_type(void) { return plugin_type_; }

  /**
   * @brief
   * Set a type of the plugin.
   * @param plugin_type [in] type of the plugin.
   */
  void set_plugin_type(PluginType plugin_type) { plugin_type_ = plugin_type; }

  /**
   * @brief
   * Add a new plugin to the list of next plugins of this plugin.
   * @param next [in] Pointer to the new plugin.
   */
  void AddNextPlugin(IPlugin* next) { next_plugins_.push_back(next); }

  /**
   * @brief
   * Get the list of next plugins.
   * @return list of next plugins.
   */
  std::vector<IPlugin*> next_plugins(void) { return next_plugins_; }

  /**
   * @brief
   * Clear the list of next plugins of this plugin.
   */
  void ClearNextPlugins(void) { next_plugins_.clear(); }

  /**
   * @brief
   * Remove a target plugin from the list of next plugins of this plugin.
   * @param plugin_name [in] plugin name of the target plugin.
   */
  void ClearNextPlugins(const std::string plugin_name) {
    if (next_plugins_.size() > 0) {
      for (unsigned int i = 0; i < next_plugins_.size(); i++) {
        if (next_plugins_[i]->plugin_name() == plugin_name) {
          next_plugins_.erase(next_plugins_.begin() + i);
          break;
        }
      }
    }
  }

  /**
   * @brief
   * Set input image size.
   * @param input_size [in] input image size.
   */
  void set_input_image_size(CvSize input_size) {
    input_image_size_ = input_size;
    output_image_size_ = input_image_size_;
  }

  /**
   * @brief
   * Get output image size.
   * @return output image size.
   */
  virtual CvSize output_image_size(void) { return output_image_size_; }

  /**
   * @brief
   * Clear the list of next plugins of this plugin.
   * This function removes a connection of each plugin recursively.
   */
  void RemoveNextPlugin(void) {
    if (next_plugins_.size() > 0) {
      for (unsigned int i = 0; i < next_plugins_.size(); i++) {
        next_plugins_[i]->RemoveNextPlugin();
      }
      next_plugins_.clear();
    }
    for (unsigned int i = 0; i < output_port_candidate_specs_.size(); i++) {
      PortSpec* output_port = output_port_candidate_specs_[i];
      output_port->set_available(true);
    }
  }

  /**
   * @brief
   * Remove a target plugin from the list of next plugins of this plugin.
   * This function removes a connection of each plugin recursively.
   * @param plugin_name [in] plugin name of the target plugin.
   */
  void RemoveNextPlugin(const std::string plugin_name) {
    if (next_plugins_.size() > 0) {
      for (unsigned int i = 0; i < next_plugins_.size(); i++) {
        if (next_plugins_[i]->plugin_name() == plugin_name) {
          next_plugins_[i]->RemoveNextPlugin();
          next_plugins_.erase(next_plugins_.begin() + i);
          break;
        }
      }
    }
  }

  /**
   * @brief
   * Get the list of the intput port.
   * @return list of the input port
   */
  std::vector<PortSpec*> input_port_candidate_specs(void) {
    return input_port_candidate_specs_;
  }

  /**
   * @brief
   * Get a list of the outtput port.
   * @return return list of the output port
   */
  std::vector<PortSpec*> output_port_candidate_specs(void) {
    return output_port_candidate_specs_;
  }

  /**
   * @brief
   * Get the list of port relation.
   * @return list of port relation.
   */
  std::vector<PortRelation*> port_relations(void) { return port_relations_; }

  /**
   * @brief
   * Get the output port spec that is currently active.
   * @return output port spec.
   */
  PortSpec* output_port_spec(void) {
    if (active_output_port_spec_index_ < output_port_candidate_specs_.size()) {
      PortSpec* port_spec =
          output_port_candidate_specs_[active_output_port_spec_index_];
      if (port_spec->available()) {
        return port_spec;
      } else {
        return NULL;
      }
    } else {
      return NULL;
    }
  }

  /**
   * @brief
   * Set the active output port spec.
   * @param index [in] Index of output port specs.
   */
  void set_active_output_port_spec_index(unsigned int index) {
    active_output_port_spec_index_ = index;
  }

  /**
   * @brief
   * Get the index of output port specs.
   * @return index of output port specs.
   */
  unsigned int active_output_port_spec_index(void) {
    return active_output_port_spec_index_;
  }

  /**
   * @brief
   * Set a original plugin name to the cloned plugin.
   * And then, clone flag set to  true.
   * @param original_plugin_name [in] original plugin name
   */
  void SetCloneParameter(std::string original_plugin_name) {
    original_plugin_name_ = original_plugin_name;
    is_cloned_ = true;
  }

  /**
   * @brief
   * Whether the plugin is cloned or not.
   * @return true, cloned plugin.
   */
  bool is_cloned(void) { return is_cloned_; }

  /**
   * @brief
   * Get original plugin name of cloned plugin.
   * @return original plugin name.
   */
  std::string original_plugin_name(void) { return original_plugin_name_; }

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
  virtual void OpenSettingWindow(ImageProcessingState state) {
    image_processing_state_ = state;
    wxString message(plugin_name().c_str(), wxConvUTF8);
    message += wxT(" plugin has no setting dialog.");
    wxMessageDialog dialog(NULL, message, wxT("Settings"), wxOK | wxSTAY_ON_TOP,
                           wxPoint(100, 100));
    if (dialog.ShowModal() == wxID_OK) {
    }
  }

  /**
   * @brief
   * Close setting window of the plugin.
   * @param state [in] ImageProcessingState.
   */
  virtual void CloseSettingWindow(ImageProcessingState state) {
    return;
  }

  /**
   * @brief
   * Set the image processing state.
   * @param state [in] ImageProcessingState.
   */
  virtual void set_image_processing_state(ImageProcessingState state) {
    image_processing_state_ = state;
  }

  /**
   * @brief
   * Get image processing state.
   * @return ImageProcessingState.
   */
  ImageProcessingState image_processing_state(void) {
    return image_processing_state_;
  }

  /**
   * @brief
   * Whether using destination buffer or not.
   * @return true, use destination buffer
   */
  bool is_use_dest_buffer(void) { return is_use_dest_; }

  /**
   * @brief
   * Get the plugin interface version
   * @return plugin interface version.
   */
  unsigned int plugin_interface_version(void) {
    return PLUGIN_INTERFACE_VERSION;
  }

  /**
   * @brief
   * Set the logger function.
   * @param logger_func [in] pointer of log output function.
   */
  void set_logger_func(void* logger_func) { logger_func_ = logger_func; }

  /**
   * @brief
   * Set output image size.
   * @param output_size [in] output image size.
   */
  void set_output_image_size(CvSize output_size) {
    output_image_size_ = output_size;
  }

  /**
   * @brief
   * Change the active index of output port spec candidates.
   * @param index [in] active index of output port spec.
   * @return true, success to change of the output port
   */
  bool ChangeOutputPortSpec(unsigned int index) {
    unsigned int output_port_num = output_port_candidate_specs_.size();
    if (0 <= index && index < output_port_num) {
      PortSpec* change_output_port_spec = output_port_candidate_specs_[index];
      if (change_output_port_spec->available()) {
        if (active_output_port_spec_index_ != index) {
          active_output_port_spec_index_ = index;
          DEBUG_PRINT("change output port index = %d, active_index = %d\n",
                      index, active_output_port_spec_index_);
        } else {
          DEBUG_PRINT("Not change output port index = %d, active_index = %d\n",
                      index, active_output_port_spec_index_);
        }
        return true;
      }
    }
    DEBUG_PRINT("ChangeOutputPortSpec indexout of range");
    return false;
  }

  /**
   * @brief
   * Set the processing time(DoProcess).
   * Calculate the average processing time for 10 times average.
   * @param time [in] processing time
   */
  void set_proc_time(float time) {
    proc_time_counter_++;
    sum_proc_time_ += time;
    if (proc_time_counter_ == 10) {
      proc_time_ = sum_proc_time_ / 10;
      sum_proc_time_ = 0.0f;
      proc_time_counter_ = 0;
    }
  }

  /**
   * @brief
   * Get the most recently measured processing time.
   * @return processing time for 10 times.
   */
  float proc_time(void) { return proc_time_; }

  /**
   * @brief
   * Get the list of parameter setting string defined by each plugin to save the
   * parameter of the plugin.
   * @return list of string.
   */
  virtual std::vector<wxString> GetPluginSettings(void) {
    return setting_params_;
  }

  /**
   * @brief
   * Set the list of parameter setting string defined by each plugin to save the
   * parameter of the plugin.
   * @param params [in] list of string.
   */
  virtual void SetPluginSettings(std::vector<wxString> params) {
    setting_params_.clear();
    setting_params_ = params;
  }

  /**
   * @brief
   * Clear the list of parameter setting string.
   * @param [out] parameter values
   */
  virtual void ClearPluginSettings(void) { setting_params_.clear(); }

  /**
   * @brief
   * Add a string to the list of parameter setting string.
   * @param [in] string with the parameter values.
   */
  virtual void AddLinePluginSettings(wxString params) {
    setting_params_.push_back(params);
  }

 protected:
  /**
   * @brief
   * Add the input candidate port spec.
   * @param type [in] PlaneType
   * @return input port candidate spec id
   */
  int AddInputPortCandidateSpec(PlaneType type) {
    PortSpec* port_spec;
    port_spec = new PortSpec(type);
    input_port_candidate_specs_.push_back(port_spec);
    return input_port_candidate_specs_.size() - 1;
  }

  /**
   * @brief
   * Add the output candidate port spec.
   * @param type [in] PlaneType
   * @return output port candidate spec id
   */
  int AddOutputPortCandidateSpec(PlaneType type) {
    PortSpec* port_spec;
    port_spec = new PortSpec(type);
    output_port_candidate_specs_.push_back(port_spec);
    return output_port_candidate_specs_.size() - 1;
  }

  /**
   * @brief
   * Add the port relation by input/output port candidate spec id
   * @param input_index [in] input port candidate spec id
   * @param output_index [in] output port candidate spec id
   * @return true, success in additional port relation
   */
  bool AddPortRelation(unsigned int input_index, unsigned int output_index) {
    PortSpec* input;
    PortSpec* output;
    PortRelation* port_relation;

    // Check input_port_candidate_specs range.
    if (input_index < input_port_candidate_specs_.size()) {
      input = input_port_candidate_specs_[input_index];
    } else {
      // Log(out of range input_port_candidate_specs)
      return false;
    }
    // Check output_port_candidate_specs range.
    if (output_index < output_port_candidate_specs_.size()) {
      output = output_port_candidate_specs_[output_index];
    } else {
      // Log(out of range output_port_candidate_specs)
      return false;
    }

    // Check duplicated port relation
    for (unsigned int i = 0; i < port_relations_.size(); i++) {
      PortRelation* tmp_relation = port_relations_[i];
      PortSpec* tmp_input = tmp_relation->input_port;
      PortSpec* tmp_output = tmp_relation->output_port;
      if (tmp_input->plane_type() == input->plane_type() &&
          tmp_output->plane_type() == output->plane_type()) {
        // Log
        return false;
      }
    }

    // Create and add port relation.
    port_relation = new PortRelation();
    port_relation->input_port = input;
    port_relation->output_port = output;
    port_relations_.push_back(port_relation);
    return true;
  }

  /**
   * @brief
   * Set whether to use the dest buffer in output.
   * @param is_use_dest_buffer [in] if true, use dest buffer
   */
  void set_is_use_dest_buffer(bool is_use_dest) { is_use_dest_ = is_use_dest; }
};

typedef void (*LogFunc)(enum LogLevel, wxString, wxString, ...);
typedef PluginBase* PluginBaseCreate(void);
typedef void PluginBaseDestroy(PluginBase* plugin_base);

#define PLUGIN_LOG_FATAL_ERROR(message, ...)                    \
  if (logger_func_) {                                           \
    LogFunc func = (LogFunc)logger_func_;                       \
    wxString name(plugin_name().c_str(), wxConvUTF8);           \
    (*func)(kLogFatalError, name, wxT(message), ##__VA_ARGS__); \
  }
#define PLUGIN_LOG_ERROR(message, ...)                     \
  if (logger_func_) {                                      \
    LogFunc func = (LogFunc)logger_func_;                  \
    wxString name(plugin_name().c_str(), wxConvUTF8);      \
    (*func)(kLogError, name, wxT(message), ##__VA_ARGS__); \
  }
#define PLUGIN_LOG_WARNING(message, ...)                     \
  if (logger_func_) {                                        \
    LogFunc func = (LogFunc)logger_func_;                    \
    wxString name(plugin_name().c_str(), wxConvUTF8);        \
    (*func)(kLogWarning, name, wxT(message), ##__VA_ARGS__); \
  }
#define PLUGIN_LOG_MESSAGE(message, ...)                     \
  if (logger_func_) {                                        \
    LogFunc func = (LogFunc)logger_func_;                    \
    wxString name(plugin_name().c_str(), wxConvUTF8);        \
    (*func)(kLogMessage, name, wxT(message), ##__VA_ARGS__); \
  }
#define PLUGIN_LOG_VERBOSE(message, ...)                     \
  if (logger_func_) {                                        \
    LogFunc func = (LogFunc)logger_func_;                    \
    wxString name(plugin_name().c_str(), wxConvUTF8);        \
    (*func)(kLogVerbose, name, wxT(message), ##__VA_ARGS__); \
  }
#define PLUGIN_LOG_STATUS(message, ...)                     \
  if (logger_func_) {                                       \
    LogFunc func = (LogFunc)logger_func_;                   \
    wxString name(plugin_name().c_str(), wxConvUTF8);       \
    (*func)(kLogStatus, name, wxT(message), ##__VA_ARGS__); \
  }
#define PLUGIN_LOG_DEBUG(message, ...)                     \
  if (logger_func_) {                                      \
    LogFunc func = (LogFunc)logger_func_;                  \
    wxString name(plugin_name().c_str(), wxConvUTF8);      \
    (*func)(kLogDebug, name, wxT(message), ##__VA_ARGS__); \
  }
#endif /* _PLUGIN_BASE_ */
