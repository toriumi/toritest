/**
 * @file      plugin_edit_item_info.cpp
 * @brief     Source for PluginEditItemInfo class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./plugin_edit_item_info.h"
#include <math.h>
#include <string>
#include <vector>

/**
 * @brief
 * Constructor.
 */
PluginEditItemInfo::PluginEditItemInfo(void) {
  DEBUG_PRINT("PluginEditItemInfo::PluginEditItemInfo()\n");

  /*************Initialize*************/
  position_ = wxPoint(-1, -1);
  plugin_name_ = "";
  scroled_x_ = 0;
  scroled_y_ = 0;

  /*************State*************/
  plugin_add_enable_state_ = kEnableToAddAllPlugin;

  /*************Load bitmap*************/
  bitmap_plus_.LoadFile(wxT(kBitmapPlusPath), wxBITMAP_TYPE_BMP);
  bitmap_plus_selected_.LoadFile(wxT(kBitmapPlusSelectedPath),
                                 wxBITMAP_TYPE_BMP);
  bitmap_plus_invalid_.LoadFile(wxT(kBitmapPlusInvalidPath), wxBITMAP_TYPE_BMP);
  bitmap_plugin_.LoadFile(wxT(kBitmapPluginPath), wxBITMAP_TYPE_BMP);
  set_selected(false);
  set_state(kStop);
  // Initialize
  prev_item_info_ = NULL;
  next_item_info_.clear();
  current_plugin_setting_state_ = kNotSetPlugin;
}

/**
 * @brief
 * Destructor.
 */
PluginEditItemInfo::~PluginEditItemInfo() {
  DEBUG_PRINT("PluginEditItemInfo::~PluginEditItemInfo()\n");
}

/**
 * @brief
 * Set position on plugin edit canvas.
 * @param position [in] position.
 */
void PluginEditItemInfo::set_position(wxPoint position) {
  position_ = position;

  SetAddPluginRect();
  SetCurrentPluginRect();
  SetAddCycleRect();
}

/**
 * @brief
 * Get position on plugin edit canvas.
 * return position.
 */
wxPoint PluginEditItemInfo::position(void) { return position_; }

/**
 * @brief
 * Shift position on plugin edit canvas.
 * When new plugin added before this item,
 * it is necessary to shift position by this function or ShiftPositionAllNext.
 * @param x [in] shift x
 * @param y [in] shift y
 */
void PluginEditItemInfo::ShiftPosition(int x, int y) {
  position_.x += x;
  position_.y += y;

  SetAddPluginRect();
  SetCurrentPluginRect();
  SetAddCycleRect();
}

/**
 * @brief
 * Shift current and all next items position on plugin edit canvas.
 * When new plugin added before this item,
 * it is necessary to shift position by this function or ShiftPosition.
 * @param x [in] shift x
 * @param y [in] shift y
 * return plugin names that were update position.
 */
std::vector<std::string> PluginEditItemInfo::ShiftPositionAllNext(int x,
                                                                  int y) {
  std::vector<std::string> next_plugin_names;
  std::vector<std::string> temp_plugin_names;
  std::string temp_plugin_name = "";

  ShiftPosition(x, y);
  if (next_item_info_.size() > 0) {
    for (unsigned int i = 0; i < next_item_info_.size(); i++) {
      temp_plugin_names = next_item_info_[i]->ShiftPositionAllNext(x, y);
      for (int k = 0; k < temp_plugin_names.size(); k++) {
        next_plugin_names.push_back(temp_plugin_names[k]);
      }
    }
  }
  next_plugin_names.push_back(this->plugin_name());
  return next_plugin_names;
}

/**
 * @brief
 * Check the Add plugin pane("+") of this item touched.
 * @param x [in] x position
 * @param y [in] y position
 * return true : touch.
 *        false: do not touch.
 */
bool PluginEditItemInfo::IsTouchAddPluginPane(int x, int y) {
  bool ret = false;

  if ((current_plugin_setting_state_ == kNotSetPlugin) ||
      (current_plugin_setting_state_ == kBranchLineDummyPlugin)) {
    return ret;
  }

  if ((rect_plus_plugin_button_.x < x) &&
      ((rect_plus_plugin_button_.x + rect_plus_plugin_button_.width) > x) &&
      (rect_plus_plugin_button_.y < y) &&
      ((rect_plus_plugin_button_.y + rect_plus_plugin_button_.height) > y)) {
    DEBUG_PRINT("Hit AddPluginPane!!\n");
    set_selected(true);
    ret = true;
  } else {
    DEBUG_PRINT("Not Hit AddPluginPane!!\n");
    set_selected(false);
  }

  return ret;
}

/**
 * @brief
 * Check the current plugin pane(Circle) of this item touched.
 * @param x [in] x position
 * @param y [in] y position
 * return true : touch.
 *        false: do not touch.
 */
bool PluginEditItemInfo::IsTouchCurrentPluginPane(int x, int y) {
  bool ret = false;

  if ((current_plugin_setting_state_ == kNotSetPlugin) ||
      (current_plugin_setting_state_ == kBranchPointDummyPlugin) ||
      (current_plugin_setting_state_ == kBranchLineDummyPlugin)) {
    return ret;
  }

  double diffPowX = pow(center_point_current_plugin_button_.x - x, 2);
  double diffPowY = pow(center_point_current_plugin_button_.y - y, 2);
  double powLength = pow(kPluginIconW, 2);

  if (diffPowX + diffPowY < powLength) {
    DEBUG_PRINT("Hit CurrentPluginPane!!\n");
    ret = true;
  }

  return ret;
}

/**
 * @brief
 * Check the Cycle pane of this item touched.
 * @param x [in] x position
 * @param y [in] y position
 * return true : touch.
 *        false: do not touch.
 */
bool PluginEditItemInfo::IsTouchCyclePane(int x, int y) {
  bool ret = false;

  if (current_plugin_setting_state_ == kNotSetPlugin) {
    return ret;
  }

  if ((current_plugin_setting_state_ == kBranchLineDummyPlugin) &&
      (prev_item_info_ != NULL) &&
      ((position_.y - prev_item_info_->position_.y) != 0)) {
    if ((rect_cycle_button_.x < x) &&
        ((rect_cycle_button_.x + rect_cycle_button_.width) > x) &&
        (rect_cycle_button_.y < y) &&
        ((rect_cycle_button_.y + rect_cycle_button_.height) > y)) {
      DEBUG_PRINT("Hit AddCyclePane!!\n");
      ret = true;
    } else {
      DEBUG_PRINT("Not Hit AddCyclePane!!\n");
    }
  }

  return ret;
}

/**
 * @brief
 * Set rect of the Add plugin pane("+").
 */
void PluginEditItemInfo::SetAddPluginRect(void) {
  if (position_.x < 0 || position_.y < 0) {
    return;
  }

  wxRect pos = wxRect(
      kFirstPluginStartX + kOnePluginPaneWidth * position_.x - scroled_x_,
      kFirstPluginStartY + kOnePluginPaneHeight * position_.y - scroled_y_,
      kAddPluginIconW, kAddPluginIconH);
  rect_plus_plugin_button_ = pos;
}

/**
 * @brief
 * Set rect of the Cycle pane.
 */
void PluginEditItemInfo::SetAddCycleRect(void) {
  if ((current_plugin_setting_state_ == kBranchLineDummyPlugin) &&
      (prev_item_info_ != NULL) &&
      (position_.y - prev_item_info_->position_.y) != 0) {
    rect_cycle_button_ = wxRect(center_point_current_plugin_button_.x,
                                center_point_current_plugin_button_.y,
                                kPluginIconW * 2, kPluginIconW);
  } else {
    rect_cycle_button_ = wxRect(-1, -1, -1, -1);
  }
}

/**
 * @brief
 * Set rect of the current plugin pane(Circle).
 */
void PluginEditItemInfo::SetCurrentPluginRect(void) {
  if (position_.x < 0 || position_.y < 0) {
    return;
  }

  wxPoint pos =
      wxPoint(kFirstPluginStartX + kAddPluginIconW + kIconIntervalW +
                  kPluginIconW + kOnePluginPaneWidth * position_.x - scroled_x_,
              kFirstPluginStartY + kAddPluginIconH / 2 +
                  kOnePluginPaneHeight * position_.y - scroled_y_);
  center_point_current_plugin_button_ = pos;
}

/**
 * @brief
 * set plugin name.
 * @param plugin_name [in] plugin name of this item.
 */
void PluginEditItemInfo::set_plugin_name(std::string plugin_name) {
  plugin_name_ = plugin_name;

  /*************State*************/
  plugin_add_enable_state_ = kEnableToAddAllPlugin;
}

/**
 * @brief
 * Draw this item's information.
 * info:Three parameters ware prepared, but at present we use only dc_circle.
 * @param dc_line [in] device context for draw line.
 * @param dc_rectangle [in] device context for draw rectangle.
 * @param dc_circle [in] device context for draw circle.
 */
void PluginEditItemInfo::Draw(wxPaintDC& dc_line, wxPaintDC& dc_rectangle,
                              wxPaintDC& dc_circle) {
  if (current_plugin_setting_state_ == kNotSetPlugin) {
    return;
  }

  int coeffX = 100 * position_.x;
  int coeffY = 100 * position_.y;

  if (current_plugin_setting_state_ == kSetBlankPlugin) {
    // +
    if (image_proc_state_ == kStop) {
      if (is_selected_) {
        dc_circle.DrawBitmap(bitmap_plus_selected_, rect_plus_plugin_button_.x,
                             rect_plus_plugin_button_.y, false);
      } else {
        dc_circle.DrawBitmap(bitmap_plus_, rect_plus_plugin_button_.x,
                             rect_plus_plugin_button_.y, false);
      }
    } else {
      dc_circle.DrawBitmap(bitmap_plus_invalid_, rect_plus_plugin_button_.x,
                           rect_plus_plugin_button_.y, false);
    }
    // + -
    dc_circle.DrawLine(
        wxPoint(rect_plus_plugin_button_.x + kAddPluginIconW,
                rect_plus_plugin_button_.y + kAddPluginIconH / 2),
        wxPoint(rect_plus_plugin_button_.x + kAddPluginIconW + kIconIntervalW,
                rect_plus_plugin_button_.y + kAddPluginIconH / 2));
  } else if (current_plugin_setting_state_ == kSetValidPlugin) {
    if (plugin_add_enable_state_ & kEnableToAddPrevPlugin) {
      // +
      if (image_proc_state_ == kStop) {
        if (is_selected_) {
          dc_circle.DrawBitmap(bitmap_plus_selected_,
                               rect_plus_plugin_button_.x,
                               rect_plus_plugin_button_.y, false);
        } else {
          dc_circle.DrawBitmap(bitmap_plus_, rect_plus_plugin_button_.x,
                               rect_plus_plugin_button_.y, false);
        }
      } else {
        dc_circle.DrawBitmap(bitmap_plus_invalid_, rect_plus_plugin_button_.x,
                             rect_plus_plugin_button_.y, false);
      }
      // + -
      dc_circle.DrawLine(
          wxPoint(rect_plus_plugin_button_.x + kAddPluginIconW,
                  rect_plus_plugin_button_.y + kAddPluginIconH / 2),
          wxPoint(rect_plus_plugin_button_.x + kAddPluginIconW + kIconIntervalW,
                  rect_plus_plugin_button_.y + kAddPluginIconH / 2));
    }
    // Draw [O -] image
    dc_circle.DrawBitmap(
        bitmap_plugin_, center_point_current_plugin_button_.x - kPluginIconW,
        center_point_current_plugin_button_.y - kPluginIconW, false);

    if (plugin_add_enable_state_ & kEnableToAddNextPlugin) {
      dc_circle.DrawLine(
          wxPoint(center_point_current_plugin_button_.x + kPluginIconW,
                  center_point_current_plugin_button_.y),
          wxPoint(center_point_current_plugin_button_.x + kPluginIconW +
                      kIconIntervalW,
                  center_point_current_plugin_button_.y));
    }
    wxString name(plugin_name_.c_str(), wxConvUTF8);
    dc_circle.DrawText(
        name, center_point_current_plugin_button_.x - kPluginIconW,
        center_point_current_plugin_button_.y - kPluginIconW * 2);
  } else if (current_plugin_setting_state_ == kBranchPointDummyPlugin) {
    // +
    if (image_proc_state_ == kStop) {
      if (is_selected_) {
        dc_circle.DrawBitmap(bitmap_plus_selected_, rect_plus_plugin_button_.x,
                             rect_plus_plugin_button_.y, false);
      } else {
        dc_circle.DrawBitmap(bitmap_plus_, rect_plus_plugin_button_.x,
                             rect_plus_plugin_button_.y, false);
      }
    } else {
      dc_circle.DrawBitmap(bitmap_plus_invalid_, rect_plus_plugin_button_.x,
                           rect_plus_plugin_button_.y, false);
    }
    // + -
    dc_circle.DrawLine(wxPoint(center_point_current_plugin_button_.x -
                                   kPluginIconW - kIconIntervalW,
                               center_point_current_plugin_button_.y),
                       wxPoint(center_point_current_plugin_button_.x +
                                   kAddPluginIconW + kPluginIconW,
                               center_point_current_plugin_button_.y));
  } else if (current_plugin_setting_state_ == kBranchLineDummyPlugin) {
    dc_circle.DrawLine(
        wxPoint(
            center_point_current_plugin_button_.x - kPluginIconW -
                kIconIntervalW,
            center_point_current_plugin_button_.y -
                kIconIntervalH * (position_.y - prev_item_info_->position_.y)),
        wxPoint(center_point_current_plugin_button_.x + kAddPluginIconW +
                    kPluginIconW,
                center_point_current_plugin_button_.y));

    dc_circle.DrawRectangle(rect_cycle_button_);
    dc_circle.DrawText(wxT("cycle"), rect_cycle_button_.x + kCycleTextOffsetX,
                       rect_cycle_button_.y + kCycleTextOffsetY);
  } else if (current_plugin_setting_state_ == kDeleteDummyPlugin) {
  }
}

/**
 * @brief
 * Update un-scrolled position.
 * when plugin edit canvas scrolled,
 * it is neccesary update position by this function.
 * @param scroled_x [in] scrolled x.
 * @param scroled_y [in] scrolled y.
 */
void PluginEditItemInfo::UpdateUnScrolledPosition(int scroled_x,
                                                  int scroled_y) {
  scroled_x_ = scroled_x;
  scroled_y_ = scroled_y;

  SetAddPluginRect();
  SetCurrentPluginRect();
  SetAddCycleRect();
}

/**
 * @brief
 * Get height from this item and next items.
 * return height on plugin edit canvas.
 */
int PluginEditItemInfo::GetBranchHeight(void) {
  int height;

  DEBUG_PRINT("PluginEditItemInfo size %d \n", next_item_info_.size());
  if (next_item_info_.size() <= 0) {
    height = position_.y;
  } else {
    height = next_item_info_[next_item_info_.size() - 1]->GetBranchHeight();
  }

  return height;
}
