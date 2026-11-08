#include "main.h"

namespace robot {

lemlib::Drivetrain drivetrain_dimensions(
    &drivetrain_left_motors,
    &drivetrain_right_motors,
    10.720, // Track width (https://kb.vex.com/hc/en-us/articles/360057008811)
    lemlib::Omniwheel::NEW_325,
    450,    // Output speed, in RPM
    2.5     // Horizontal drift (omni → lower, traction → higher), out of 10?
);

lemlib::ControllerSettings drivetrain_linear_PID(
    5.0,    // Proportional gain (kP)
    0.0,    // Integral gain (kI)
    10.0,    // Derivative gain (kD)
    0.0,    // Integral start range, in inches
    
    0.50,   // Small exit range, in inches
    125,    // Small exit duration, in milliseconds
    1.50,   // Large exit range, in inches
    500,    // Large exit duration, in milliseconds

    100     // Max acceleration, out of 127
);

lemlib::ControllerSettings drivetrain_angular_PID(
    1.7,    // Proportional gain (kP)
    0.0,    // Integral gain (kI)
    10.0,    // Derivative gain (kD)
    0.0,    // Integral start range, in degrees
    
    2.00,   // Small exit range, in degrees
    125,    // Small exit duration, in milliseconds
    5.00,   // Large exit range, in degrees
    400,    // Large exit duration, in milliseconds

    100      // Max acceleration, out of 127
);

lemlib::OdomSensors drivetrain_sensors(
    nullptr, 
    nullptr,
    nullptr,
    nullptr,
    &(robot::drivetrain_inertial_sensor)
);

lemlib::ExpoDriveCurve drivetrain_throttle_curve(
    3.0,  // Joystick deadband, out of 127
    5.0,  // Minimum velocity, out of 127
    1.02  // Curvature (https://www.desmos.com/calculator/umicbymbnl)
);

lemlib::ExpoDriveCurve drivetrain_steering_curve(
    3.0,  // Joystick deadband, out of 127
    5.0,  // Minimum velocity, out of 127
    1.02  // Curvature (https://www.desmos.com/calculator/umicbymbnl)
);

lemlib::ControllerSettings lift_PID(
    0.7,    // Proportional gain (kP)
    0.0,    // Integral gain (kI)
    3.0,    // Derivative gain (kD)
    0.0,    // Integral start range, in degrees
    
    2.00,   // Small exit range, in degrees
    125,    // Small exit duration, in milliseconds
    8.00,   // Large exit range, in degrees
    375,    // Large exit duration, in milliseconds

    55      // Max acceleration, out of 127
);

} // namespace robot