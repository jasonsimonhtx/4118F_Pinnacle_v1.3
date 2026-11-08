#include "main.h"

namespace {

    using ControllerJoystick = pros::controller_analog_e_t;
    using ControllerButtons = std::vector<pros::controller_digital_e_t>;

    const std::pair<int8_t, int8_t> THROTTLE_LIMITS = std::make_pair<int8_t, int8_t>(-110, +110);
    const std::pair<int8_t, int8_t> STEERING_LIMITS = std::make_pair<int8_t, int8_t>(-100, +100);

    const ControllerJoystick THROTTLE_JOYSTICK = pros::E_CONTROLLER_ANALOG_LEFT_Y;
    const ControllerJoystick STEERING_JOYSTICK = pros::E_CONTROLLER_ANALOG_RIGHT_X;

    const ControllerButtons LIFT_RAISE_BUTTONS = {
        pros::E_CONTROLLER_DIGITAL_L1
    };

    const ControllerButtons LIFT_LOWER_BUTTONS = {
        pros::E_CONTROLLER_DIGITAL_L2
    };

    const ControllerButtons CLAW_INTAKE_BUTTONS = {
        pros::E_CONTROLLER_DIGITAL_R1
    };

    const ControllerButtons CLAW_RELEASE_BUTTONS = {
        pros::E_CONTROLLER_DIGITAL_R2
    };

    bool is_button_pressed(const ControllerButtons& buttons) {
        for (const auto& button : buttons) {
            if (controller.get_digital(button)) {
                return true;
            }
        }
        return false;
    }

} // namespace anonymous

/**
 * @brief Custom function to initiate operator control.
 */
void startOperatorControl() {
    while (!pros::competition::is_disabled() && !pros::competition::is_autonomous()) {
        // Update start time
        uint32_t startTime = pros::millis();

        // Motor control for chassis
		auto throttle = std::clamp((int8_t)controller.get_analog(THROTTLE_JOYSTICK), (int8_t)THROTTLE_LIMITS.first, (int8_t)THROTTLE_LIMITS.second);
		auto steering = std::clamp((int8_t)controller.get_analog(STEERING_JOYSTICK), (int8_t)STEERING_LIMITS.first, (int8_t)STEERING_LIMITS.second);
        chassis.arcade(throttle, steering, false, 0.5F); // Update chassis motors

        // Motor control for lift
        if (is_button_pressed(LIFT_RAISE_BUTTONS)) {
            lift.set_target_velocity(600.0);
        } else if (is_button_pressed(LIFT_LOWER_BUTTONS)) {
            lift.set_target_velocity(-600.0);
        } else {
            lift.set_target_velocity(0.0);
        }
        
        // Motor control for claw
        if (is_button_pressed(CLAW_INTAKE_BUTTONS)) {
            claw.move_velocity(+200);
            claw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        } else if (is_button_pressed(CLAW_RELEASE_BUTTONS)) {
            claw.move_velocity(-200);
            claw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        }

        if (controller.get_digital_new_release(CLAW_INTAKE_BUTTONS[0])) {
            claw.move_velocity(+25);
            claw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        } else if (controller.get_digital_new_release(CLAW_RELEASE_BUTTONS[0])) {
            claw.move_velocity(-25);
            claw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        }

        pros::c::task_delay_until(&startTime, 10);
    }
}