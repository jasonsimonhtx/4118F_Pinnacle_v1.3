#ifndef _ROBOT_HARDWARE_HPP_
#define _ROBOT_HARDWARE_HPP_

#include "main.h"

namespace robot {

extern pros::Controller   controller;

extern pros::MotorGroup   drivetrain_left_motors;
extern pros::MotorGroup   drivetrain_right_motors;
extern /*subsys::ScaledIMU*/ pros::IMU  drivetrain_inertial_sensor;

extern pros::MotorGroup   lift_motors;
extern pros::Rotation     lift_rotation_sensor;

extern pros::Motor        claw_motor;

extern lemlib::Chassis*   drivetrain;
/*
extern subsys::LiftController*  lift;
extern subsys::ClawController*  claw;
*/

} // namespace robot

#endif // _ROBOT_HARDWARE_HPP_