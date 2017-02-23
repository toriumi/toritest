/**
 * @file      plugin_edit_item_info.h
 * @brief     Header for PluginEditItemInfo class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _PLUGIN_EDIT_ITEM_INFO_H_
#define _PLUGIN_EDIT_ITEM_INFO_H_

#include <string>
#include <vector>
#include "./include.h"
#include "./plugin_base.h"
#include "./plugin_edit_canvas_define.h"
#include "wx/wx.h"

/**
 * @class PluginEditItemInfo
 * @brief The class that manage one plugin edit item on plugin flow screen.
 */
class PluginEditItemInfo {
  // private:
 public:
  PluginEditItemInfo* prev_item_info_;

  /**
   * @brief
   * Get the previous plugin name.
   * if prev is not connect return blank name.
   * return previous plugin name.
   */
  std::string GetPrevPluginName() {
    if (prev_item_info_ == NULL) {
      std::string brank_name = "";
      DEBUG_PRINT("[Debug] GetPrevPluginName() = NULL \n");
      return brank_name;
    }
    return prev_item_info_->plugin_name();
  }

  std::vector<PluginEditItemInfo*> next_item_info_;
  /**
   * @brief
   * Get the next plugin name.
   * if invalid i passed, return blank name.
   * @param i [in] num of next_item_info.
   * return next plugin name.
   */
  std::string GetNextPluginName(unsigned int i) {
    std::string brank_name = "";
    if (next_item_info_.size() <= i) {
      DEBUG_PRINT("[Debug] GetNextPluginName size(%d) <= i(%d)\n",
                  next_item_info_.size(), i);
      return brank_name;
    }

    return next_item_info_[i]->plugin_name();
  }

  /**
   * @brief
   * Find the plugin id on next_item_info from plugin name.
   * if plugin name not found return -1;
   * @param name [in] plugin name.
   * return num of next_item_info.
   */
  int FindPluginId(std::string name) {
    int ret = -1;
    for (int i = 0; i < next_item_info_.size(); i++) {
      if (name == next_item_info_[i]->plugin_name()) {
        DEBUG_PRINT("[FindPluginName]find = %d \n", i);
        ret = i;
        break;
      }
    }
    DEBUG_PRINT("[FindPluginName] %s not found \n", name.c_str());
    return ret;
  }

  PluginSettingState current_plugin_setting_state_;
  /**
   * @brief
   * Set plugin state(PluginSettingState).
   * @param state [in]  plugin state(PluginSettingState).
   */
  void set_current_plugin_setting_state(PluginSettingState state) {
    current_plugin_setting_state_ = state;
  }
  /**
   * @brief
   * Get plugin state(PluginSettingState).
   * return plugin state(PluginSettingState).
   */
  PluginSettingState current_plugin_setting_state(void) {
    return current_plugin_setting_state_;
  }

  /*! coordinates on plugin edit canvas*/
  wxPoint position_;

  /*! Rectangle of plus icon*/
  wxRect rect_plus_plugin_button_;

  /*! Bitmap of plus icon(normal state)*/
  wxBitmap bitmap_plus_;

  /*! Bitmap of plus icon(selected state)*/
  wxBitmap bitmap_plus_selected_;

  /*! Bitmap of plus icon(invalid state)*/
  wxBitmap bitmap_plus_invalid_;

  /*! Bitmap of plugin icon*/
  wxBitmap bitmap_plugin_;

  /*! Rectangle of plugin icon*/
  wxRect rect_cycle_button_;

  /*! Plugin name to display on edit canvas*/
  std::string plugin_name_;

  /*! Circle position on edit canvas*/
  wxPoint center_point_current_plugin_button_;

  /*! Scroll amount in the X direction of edit canvas*/
  int scroled_x_;

  /*! Scroll amount in the Y direction of edit canvas*/
  int scroled_y_;

  int plugin_add_enable_state_;

 public:
  /**
   * @brief
   * Constructor.
   */
  PluginEditItemInfo(void);

  /**
   * @brief
   * Destructor.
   */
  ~PluginEditItemInfo();

  /**
   * @brief
   * Set position on plugin edit canvas.
   * @param position [in] position.
   */
  void set_position(wxPoint position);

  /**
   * @brief
   * Get position on plugin edit canvas.
   * return position.
   */
  wxPoint position(void);

  /**
   * @brief
   * Shift position on plugin edit canvas.
   * When new plugin added before this item,
   * it is necessary to shift position by this function or ShiftPositionAllNext.
   * @param x [in] shift x
   * @param y [in] shift y
   */
  void ShiftPosition(int x, int y);

  /**
   * @brief
   * Shift current and all next items position on plugin edit canvas.
   * When new plugin added before this item,
   * it is necessary to shift position by this function or ShiftPosition.
   * @param x [in] shift x
   * @param y [in] shift y
   * return plugin names that were update position.
   */
  std::vector<std::string> ShiftPositionAllNext(int x, int y);

  /**
   * @brief
   * Check the Add plugin pane("+") of this item touched.
   * @param x [in] x position
   * @param y [in] y position
   * return true : touch.
   *        false: do not touch.
   */
  bool IsTouchAddPluginPane(int x, int y);

  /**
   * @brief
   * Check the current plugin pane(Circle) of this item touched.
   * @param x [in] x position
   * @param y [in] y position
   * return true : touch.
   *        false: do not touch.
   */
  bool IsTouchCurrentPluginPane(int x, int y);

  /**
   * @brief
   * Check the Cycle pane of this item touched.
   * @param x [in] x position
   * @param y [in] y position
   * return true : touch.
   *        false: do not touch.
   */
  bool IsTouchCyclePane(int x, int y);

  /**
   * @brief
   * Set rect of the Add plugin pane("+").
   */
  void SetAddPluginRect(void);

  /**
   * @brief
   * Set rect of the Cycle pane.
   */
  void SetAddCycleRect(void);

  /**
   * @brief
   * Set rect of the current plugin pane(Circle).
   */
  void SetCurrentPluginRect(void);

  /**
   * @brief
   * set plugin name.
   * @param plugin_name [in] plugin name of this item.
   */
  void set_plugin_name(std::string plugin_name);

  /**
   * @brief
   * get plugin name.
   * return plugin name of this item.
   */
  std::string plugin_name(void) { return plugin_name_; }

  /**
   * @brief
   * Draw this item's information.
   * info:Three parameters ware prepared, but at present we use only dc_circle.
   * @param dc_line [in] device context for draw line.
   * @param dc_rectangle [in] device context for draw rectangle.
   * @param dc_circle [in] device context for draw circle.
   */
  void Draw(wxPaintDC& dc_line, wxPaintDC& dc_c, wxPaintDC& dc_r);  // NOLINT

  /**
   * @brief
   * Set plugin add state(PluginAddEnableState).
   * @param state [in] add state(PluginAddEnableState).
   */
  void set_plugin_add_enable_state(int state) {
    plugin_add_enable_state_ = state;
  }

  /**
   * @brief
   * Get plugin add state(PluginAddEnableState).
   * return add state(PluginAddEnableState).
   */
  int plugin_add_enable_state(void) { return plugin_add_enable_state_; }

  /**
   * @brief
   * Set selected state of this item.
   * @param selected [in] select this item or not.
   */
  void set_selected(bool selected) { is_selected_ = selected; }

  /**
   * @brief
   * Set image processing state.
   * @param image_proc_state [in] Image processing state.
   */
  void set_state(ImageProcessingState image_proc_state) {
    image_proc_state_ = image_proc_state;
  }

  /**
   * @brief
   * Update un-scrolled position.
   * when plugin edit canvas scrolled,
   * it is neccesary update position by this function.
   * @param scroled_x [in] scrolled x.
   * @param scroled_y [in] scrolled y.
   */
  void UpdateUnScrolledPosition(int scroled_x, int scroled_y);

  /**
   * @brief
   * Get height from this item and next items.
   * return height on plugin edit canvas.
   */
  int GetBranchHeight(void);

 private:
  /*! is selected this item or not on edit canvas */
  bool is_selected_;

  /*! ImageProcessingState object */
  ImageProcessingState image_proc_state_;
};

#endif  // _PLUGIN_EDIT_ITEM_INFO_H_
