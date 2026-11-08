#ifndef _ROBOT_PARAMETERS_HPP_
#define _ROBOT_PARAMETERS_HPP_

#include "main.h"

namespace robot {

extern lemlib::Drivetrain          drivetrain_dimensions;
extern lemlib::ControllerSettings  drivetrain_linear_PID;
extern lemlib::ControllerSettings  drivetrain_angular_PID;
extern lemlib::OdomSensors         drivetrain_sensors;
extern lemlib::ExpoDriveCurve      drivetrain_throttle_curve;
extern lemlib::ExpoDriveCurve      drivetrain_steering_curve;
extern lemlib::ControllerSettings  lift_PID;

} // namespace robot

#endif // _ROBOT_PARAMETERS_HPP_