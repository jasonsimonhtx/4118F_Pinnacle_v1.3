#include "main.h"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup drivetrainLeftMotors({+13, -12, -11}, pros::MotorGearset::blue);
pros::MotorGroup drivetrainRightMotors({-18, +19, +20}, pros::MotorGearset::blue);
ScaledIMU drivetrainInertial(14, 3600.0/3600.0);

pros::MotorGroup liftMotors({+1, -10}, pros::MotorGearset::blue);
pros::Rotation liftRotationSensor(-3);
LiftController lift(&liftMotors, &liftRotationSensor,
    {0.28, 0.02, 0.8, 1.0, 11.5, 10U},
    {50.0, 3100.0}
);

pros::Motor claw(-2, pros::MotorGearset::green);

// Drivetrain dimensions wrapper
lemlib::Drivetrain drivetrainDimensions(
    &drivetrainLeftMotors,
    &drivetrainRightMotors,
    10.720, // Track width (https://kb.vex.com/hc/en-us/articles/360057008811)
    lemlib::Omniwheel::NEW_325,
    450,    // Output speed, in RPM
    2.0     // Horizontal drift (omni → lower, traction → higher), out of 10?
);

// PID controller for linear (forward/backward) motion
lemlib::ControllerSettings drivetrainLinearPID(
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

// PID controller for angular (left/right) motion
lemlib::ControllerSettings drivetrainAngularPID(
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

// Drivetrain sensors wrapper
lemlib::OdomSensors drivetrainSensors(
    nullptr, 
    nullptr,
    nullptr,
    nullptr,
    &drivetrainInertial
);

// Exponential curve for throttle input during operator control
lemlib::ExpoDriveCurve drivetrainThrottleCurve(
    4.0,   // Joystick deadband, out of 127
    8.0,   // Minimum velocity, out of 127
    1.027  // Curvature (https://www.desmos.com/calculator/umicbymbnl)
);

// Exponential curve for steering input during operator control
lemlib::ExpoDriveCurve drivetrainSteeringCurve(
    4.0,   // Joystick deadband, out of 127
    8.0,   // Minimum velocity, out of 127
    1.027  // Curvature (https://www.desmos.com/calculator/umicbymbnl)
);

// Finally, create the chassis
lemlib::Chassis chassis(
    drivetrainDimensions,
    drivetrainLinearPID,
    drivetrainAngularPID,
    drivetrainSensors,
    &drivetrainThrottleCurve,
    &drivetrainSteeringCurve
);

/**
 * @brief Initialize devices and subsystem controllers.
 */
void subsystemInit() {
    // Initialize subsystem controller of the chassis
    pros::Task chassisInit([&](){
        chassis.cancelAllMotions();
        chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
        chassis.calibrate(true);
    }, 8UL, 8192U, "subsystemInit.chassisInit");

    // Configure base devices of the claw
    claw.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    claw.set_current_limit_all(2500); // Maximum limit (mA); same as unset
    claw.set_encoder_units_all(pros::E_MOTOR_ENCODER_DEGREES);
    claw.set_voltage_limit_all(12000); // Maximum limit (mV); same as unset
    claw.tare_position_all();

    // Close the claw to lock the pin for autonomous
    pros::Task clawInit([&](){
        claw.move_velocity(+150);
        pros::Task::delay(500);
        claw.brake();
    }, 8UL, 8192U, "subsystemInit.clawInit");

    // Yield until other tasks are finished
    chassisInit.join();
}