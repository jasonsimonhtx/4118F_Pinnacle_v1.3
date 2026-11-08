#pragma once
#include "main.h"
#include "lemlib/api.hpp"
#include "subsystems/liftController.hpp"
#include "subsystems/pistonController.hpp"
#include "subsystems/scaledIMU.hpp"

extern pros::Controller controller;

extern pros::MotorGroup drivetrainLeftMotors;
extern pros::MotorGroup drivetrainRightMotors;
extern ScaledIMU drivetrainInertial;

extern pros::MotorGroup liftMotors;
extern pros::Rotation liftRotationSensor;
extern LiftController lift;

extern pros::Motor claw;

extern lemlib::Drivetrain drivetrainDimensions;
extern lemlib::ControllerSettings drivetrainLinearPID;
extern lemlib::ControllerSettings drivetrainAngularPID;
extern lemlib::OdomSensors drivetrainSensors;
extern lemlib::ExpoDriveCurve drivetrainThrottleCurve;
extern lemlib::ExpoDriveCurve drivetrainSteeringCurve;
extern lemlib::Chassis chassis;

void subsystemInit();