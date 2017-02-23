/**
 * @file      port_spec.h
 * @brief     Header for PortSpec class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _PORT_SPEC__H_
#define _PORT_SPEC__H_

#include <vector>
#include "./include.h"

/**
 * @class PortSpec
 * @brief This class about the ports of the plugin.
 */
class PortSpec {
 public:
  /**
   * @brief
   * Constructor.
   * @param plane_type [in] PlaneType
   */
  explicit PortSpec(PlaneType plane_type);

  /**
   * @brief
   * Destructor.
   */
  virtual ~PortSpec(void);

  /**
   * @brief
   * Get plane type
   * @return plane type.
   */
  virtual PlaneType plane_type(void) { return plane_type_; }

  /**
   * @brief
   * Whether port is available/unavailable.
   * @return true, port is available
   */
  virtual bool available(void) { return available_; }

  /**
   * @brief
   * Set the port available/unvailable.
   * @param available [in] if true, port is available
   */
  virtual void set_available(bool available) { available_ = available; }

 private:
  /*! Represent bit depth, number of channels, color order */
  PlaneType plane_type_;
  /*! Port of using available/unavailable */
  bool available_;
};

#endif /* _PORT_SPEC__H_*/
