#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include <cmath>
#include <vector>

/**
 * note: this file controls everything related to user interface/control as
 * separate threads/sections. this includes operator control and display.
 */

// constants + typedefs //

using BrainFont = pros::text_format_e_t;

constexpr int16_t SCREEN_W = 480;
constexpr int16_t SCREEN_H = 240;

constexpr uint32_t COLOR_BLACK = 0x000000;
constexpr uint32_t COLOR_WHITE = 0xFFFFFF;
constexpr uint32_t COLOR_LIME  = 0x44FF00;
constexpr uint32_t COLOR_RED   = 0xFF0000;
constexpr uint32_t COLOR_BLUE  = 0x0000FF;

constexpr BrainFont FONT_LARGE = pros::E_TEXT_LARGE;
constexpr BrainFont FONT_MED   = pros::E_TEXT_MEDIUM;
constexpr BrainFont FONT_SMALL = pros::E_TEXT_SMALL;

// structs + enums //

struct BrainPrintFormat {
    int16_t x_top_left;
    int16_t y_top_left;
    BrainFont font_type;
    uint32_t font_color = COLOR_WHITE;
};

struct ControllerPrintFormat {
    uint8_t line;
    uint8_t col;
};

// other helper functions //

double get_motor_group_temperature(const pros::MotorGroup& motors) {
    const std::vector<double> temperatures_celsius = motors.get_temperature_all();
    if (temperatures_celsius.empty()) {
        return PROS_ERR_F;
    }

    double total_celsius = 0.0;
    for (const double temperature_celsius : temperatures_celsius) {
        if (!std::isfinite(temperature_celsius)) {
            return PROS_ERR_F;
        }
        total_celsius += temperature_celsius;
    }

    const double average_celsius = total_celsius / temperatures_celsius.size();
    return average_celsius * 9.0 / 5.0 + 32.0;
}

double get_motor_temperature(const pros::Motor& motor) {
    const double temperature_celsius = motor.get_temperature();
    if (!std::isfinite(temperature_celsius)) {
        return PROS_ERR_F;
    }
    return temperature_celsius * 9.0 / 5.0 + 32.0;
}

// setup functions //

template <typename... Args>
void brain_print(BrainPrintFormat format, const char* text, Args... args) {
    const uint32_t previous_color = pros::screen::get_pen();
    pros::screen::set_pen(format.font_color);
    pros::screen::print(format.font_type, format.x_top_left, format.y_top_left, text, args...);
    pros::screen::set_pen(previous_color);
    // ex. brain_print({10, 10, FONT_MED}, "Text: %s", "Hi")
}

template <typename... Args>
void controller_print(ControllerPrintFormat format, const char* text, Args... args) {
    controller.print(format.line, format.col, text, args...);
    // ex. controller_print({0, 0}, "Text: %s", "Hi")
}

void brain_init() {
    pros::lcd::shutdown();
    pros::screen::set_eraser(COLOR_BLACK);
    pros::screen::set_pen(COLOR_WHITE);
	pros::screen::erase();
    pros::Task::delay(50); // to be safe
    brain_print({18, 18, FONT_LARGE, COLOR_LIME}, "4118F League Night #3");
    brain_print({18, 54, FONT_LARGE, COLOR_LIME}, "Initializing...");
}

void controller_init() {
    controller.clear();
    pros::Task::delay(50); // to be safe
    controller_print({0, 0}, "Initializing...");
}

// update functions //

void brain_periodic() {
    uint32_t _millis = pros::millis();
	uint32_t _tick = 0;
    pros::screen::erase();

    while (true) {
        // load atomic id
        const RouteID route_id = current_route_id.load();

        // drivebase data
        brain_print({0, 0,  FONT_MED}, "X-pos: %.1f\"  ", chassis.getPose().x);
        brain_print({0, 24, FONT_MED}, "Y-pos: %.2f\"  ", chassis.getPose().y);
        brain_print({0, 48, FONT_MED}, "Theta: %.2f  ", chassis.getPose().theta);

        // motor temperatures
        brain_print({240, 0,  FONT_MED}, "Drive: %.0f/%.0fF  ",
            get_motor_group_temperature(drivetrainLeftMotors), get_motor_group_temperature(drivetrainRightMotors));
        brain_print({240, 24, FONT_MED}, "Lift:  %.0fF  ", get_motor_group_temperature(liftMotors));
        brain_print({240, 48, FONT_MED}, "Claw:  %.0fF  ", get_motor_temperature(claw));

        // motor velocities
        pros::screen::erase_rect(0, 84, 220, 156);
        brain_print({0, 84, FONT_MED}, "Drive: %.0f/%.0f RPM  ",
            drivetrainLeftMotors.get_actual_velocity(), drivetrainRightMotors.get_actual_velocity());
        brain_print({0, 108, FONT_MED}, "Lift: %.1f deg/s  ", lift.get_current_velocity());
        brain_print({0, 132, FONT_MED}, "Claw: %.1f RPM  ", claw.get_actual_velocity());

        // lift data
        pros::screen::erase_rect(220, 84, 480, 156);
        brain_print({240, 84, FONT_MED},  "Pos: %.0f deg  ", lift.get_current_position());
        brain_print({240, 108, FONT_MED}, "State: %s  ", [](){
            if (lift.get_state() == LiftState::IDLE) { return "IDLE"; }
            else if (lift.get_state() == LiftState::POSITIONING) { return "POSITIONING"; }
            else if (lift.get_state() == LiftState::MANUAL) { return "MANUAL"; }
            else { return "UNINIT"; }
        }());
        brain_print({240, 132, FONT_MED}, "Target: %.0f deg  ", lift.get_target_position());

        // autonomous route
        pros::screen::erase_rect(0, 158, 480, 240);
        brain_print({0, 180, FONT_MED}, "Auton route:");
        brain_print({140, 180, FONT_MED, routes[route_id].display_color}, "(%d) %s", route_id + 1, routes[route_id].title);
        brain_print({20, 204, FONT_MED, routes[route_id].display_color}, "(%s)", routes[route_id].description);

        // iterate
        _tick++;
        pros::Task::delay_until(&_millis, 50UL);
    }
}

void controller_periodic() {
    uint32_t _millis = pros::millis();
	uint32_t _tick = 0;
    controller.clear();
    
    while (true) {
        if (_tick % 5 == 0) {
            controller_print({0, 0}, "Lift: %.0f deg     ", lift.get_current_position());
            controller_print({2, 2}, "\'X\' to cycle autos"); // ??? this doesn't display
        }

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            const RouteID next_route_id = static_cast<RouteID>(
                (current_route_id.load() + 1U) % routes.size());
            current_route_id.store(next_route_id);
        }
        
        _tick++;
        pros::Task::delay_until(&_millis, 10UL);
    }
}