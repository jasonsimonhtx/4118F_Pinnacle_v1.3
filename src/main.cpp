#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

/**
 * @brief Runs initialization code. This occurs as soon as the program
 * is started.
 *
 * @details All other competition modes are blocked by initialize;
 * it is recommended to keep execution time for this mode under a few
 * seconds.
 */
void initialize() {
	brain_init();
	controller_init();
	subsystemInit();
	lift.initialize();
	pros::Task(brain_periodic, TASK_PRIORITY_DEFAULT + 2, TASK_STACK_DEPTH_DEFAULT, "brain_periodic");
	pros::Task(controller_periodic, TASK_PRIORITY_DEFAULT + 1, TASK_STACK_DEPTH_DEFAULT, "controller_periodic");
}

/**
 * @brief Runs while the robot is disabled.
 */
void disabled() {
	// Left empty for now
}

/**
 * @brief Runs after initialize if the robot is connected to field control.
 */
void competition_initialize() {
	// Left empty for now
}

/**
 * @brief Runs when the robot enters autonomous control.
 */
void autonomous() {
	controller.rumble(".-.");
	const RouteID route_id = current_route_id.load();
	if (route_id < routes.size()) {
		routes[route_id].func();
	}
}

/**
 * @brief Runs when the robot enters operator control.
 */
void opcontrol() {
	startOperatorControl();
}