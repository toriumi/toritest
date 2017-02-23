/**
 * @file      port_spec.cpp
 * @brief     Source for PortSpec class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */
#include "./port_spec.h"

/**
 * @brief
 * Constructor.
 * @param plane_type [in] PlaneType
 */
PortSpec::PortSpec(PlaneType plane_type) { 
  plane_type_ = plane_type;
  available_ = true;
}

/**
 * @brief
 * Destructor.
 */
PortSpec::~PortSpec() { ; }
