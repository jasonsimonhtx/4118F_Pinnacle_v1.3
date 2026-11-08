#include "main.h"

void initialize(void) {
    // Temporary control logic just to initialize the robot devices
    robot::drivetrain_left_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    robot::drivetrain_right_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    robot::claw_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    robot::lift_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    robot::drivetrain_inertial_sensor.reset(true);

    // Print on the brain screen
    pros::lcd::shutdown();
    pros::screen::print(pros::E_TEXT_LARGE, 20, 20, "Test print");
}

void disabled(void) {

}

void competition_initialize(void) {

}

void autonomous(void) {

}

void opcontrol(void) {
    // Temporary control logic to print telemetry
    pros::Task([](){
        while (true) {
            uint32_t startTime = pros::millis();

            pros::screen::print(pros::E_TEXT_MEDIUM, 20, 70, "Lift rotation: %.2f  ", robot::lift_rotation_sensor.get_angle() * 100.0);
            pros::screen::print(pros::E_TEXT_MEDIUM, 20, 100, "Inertial: %.2f  ", robot::drivetrain_inertial_sensor.get_heading());
            pros::screen::print(pros::E_TEXT_MEDIUM, 20, 130, "Lift draw: %d  ", robot::lift_motors.get_current_draw_all());

            pros::c::task_delay_until(&startTime, 100);
        }
    });

    // Temporary control logic just to test the robot devices
    int32_t lift_output = 0;
    while (true) {
        uint32_t startTime = pros::millis();

        const int32_t throttle = clamp(robot::controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),
            (int32_t)-127, (int32_t)+127);
        const int32_t steering = clamp(robot::controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X),
            (int32_t)-127, (int32_t)+127);
        robot::drivetrain_left_motors.move(deadband(throttle - steering, (int32_t)3));
        robot::drivetrain_right_motors.move(deadband(throttle + steering, (int32_t)3));

        int32_t claw_output = 0;
        if (robot::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { claw_output = +127; }
        else if (robot::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { claw_output = -127; }
        robot::claw_motor.move(claw_output);

        int32_t lift_target = 0;
        if (robot::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            lift_target = 127;
        } else if (robot::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            lift_target = -127;
        }
        int32_t lift_alpha = lift_target - lift_output;
        lift_alpha = clamp(lift_alpha, (int32_t)-10, (int32_t)+10);
        lift_output += lift_alpha;
        robot::lift_motors.move(lift_output);

        pros::c::task_delay_until(&startTime, 10);
    }
}