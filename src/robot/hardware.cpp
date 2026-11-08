#include "main.h"

namespace robot {

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup drivetrain_left_motors({-1, -2, +3}, pros::MotorGears::rpm_600, pros::MotorEncoderUnits::counts);
pros::MotorGroup drivetrain_right_motors({+10, +9, -8}, pros::MotorGears::rpm_600, pros::MotorEncoderUnits::counts);
/*subsys::ScaledIMU*/ pros::IMU drivetrain_inertial_sensor(/*{{*/+06/*, 1.0L}}*/);

pros::MotorGroup lift_motors({-4, +7}, pros::MotorGears::rpm_100, pros::MotorEncoderUnits::counts);
pros::Rotation lift_rotation_sensor(-05);

pros::Motor claw_motor(-20, pros::MotorGears::rpm_200, pros::MotorEncoderUnits::counts);

/*
lemlib::Chassis* drivetrain = &lemlib::Chassis(
    drivetrain_dimensions,
    drivetrain_linear_PID,
    drivetrain_angular_PID,
    drivetrain_sensors,
    &drivetrain_throttle_curve,
    &drivetrain_steering_curve
);

subsys::LiftController* lift = &subsys::LiftController(&lift_motors, &lift_rotation_sensor, lift_PID);

subsys::ClawController* claw = &subsys::ClawController(&claw_motor);
*/

} // namespace robot