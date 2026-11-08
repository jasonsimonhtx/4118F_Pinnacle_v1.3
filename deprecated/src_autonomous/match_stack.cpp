#include "main.h"
using namespace lemlib;
using enum AngularDirection;
using enum DriveSide;

// Right-side starting position (relative to driver alliance station)
void aut_match_stack_right() {
    // Define variables:
    const std::vector<double> stack_heights = {950, 1250, 1700};
    const std::vector<double> release_heights = {50, 450, 750, 1250};

    const bool FORWARD = true, REVERSE = false;
    const auto linear_params = [](bool facing_forwards) { return MoveToPointParams {
        .forwards = facing_forwards, .maxSpeed = 80, .minSpeed = 18, .earlyExitRange = 3.5}; };
    const auto angular_params = [](bool facing_forwards, AngularDirection direction = AUTO) {
        return TurnToPointParams { .forwards = facing_forwards, .direction = direction,
            .maxSpeed = 80, .minSpeed = 18, .earlyExitRange = 5.5};};
    
    const float GOAL_X = -30.0, GOAL_Y = 16.5;
    const float LOADER_X = 12.2, LOADER_Y = -1.5;

    // Initialize subsystems:
    chassis.cancelAllMotions();
    chassis.setPose(0, 0, 180, false);
    claw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    lift.set_target_position(50);

    // Primary for-loop:
    for (uint32_t i = 0; i < stack_heights.size(); i++) {
        // Start from match loader w/ a stack (or start w/ a preload)
        // Drive & score into the alliance goal
        // Drive & intake from the match loade

        // Hold onto the stack (or preload) & raise the lift:
        new pros::Task([&](){
            lift.set_target_position(stack_heights[i]);
            pros::Task::delay(50);
            claw.move_velocity(+200);
            pros::Task::delay(200);
            uint32_t decelerate_time = pros::millis();
            for (int32_t step = 1; step <= 25; ++step) {
                pros::Task::delay_until(&decelerate_time, 10);
                claw.move_velocity(200 - step * 4);
            }
            claw.move_velocity(+100);
        }, 8UL, 8192U, "auton_claw_and_lift");
        pros::Task::delay(100);

        // Drive toward & align with the nearest alliance goal:
        chassis.moveToPoint((i == 0 ? 0.0 : LOADER_X), GOAL_Y, 1100, linear_params(REVERSE));
        chassis.waitUntilDone();
        chassis.turnToPoint(GOAL_X, GOAL_Y, 900, angular_params(FORWARD));
        chassis.waitUntilDone();
        chassis.moveToPoint(GOAL_X, (GOAL_Y - i * 0.75), 1000, linear_params(FORWARD));
        chassis.waitUntilDone();

        // Score the stack (or preload) & lower the lift:
        lift.set_target_position(release_heights[i]);
        pros::Task::delay(250);
        claw.move_velocity(i == 0 ? -80 : -100);
        chassis.tank(-10, -10, true);
        pros::Task::delay(i == 0 ? 400 : 250);
        lift.set_target_position(release_heights[0]);
        
        // Drive toward & into the nearest match loader:
        chassis.moveToPoint(LOADER_X - 2.0, GOAL_Y, 1400, linear_params(REVERSE));
        chassis.waitUntilDone();
        chassis.turnToPoint(LOADER_X - 1.0, LOADER_Y, 1100, angular_params(FORWARD));
        chassis.waitUntilDone();
        chassis.moveToPoint(LOADER_X, i == stack_heights.size() - 1 ? LOADER_Y + 5.0 : LOADER_Y,
            1100, linear_params(FORWARD));
        chassis.waitUntilDone();

        // Intake a stack from the nearest match loader:
        //if (i == stack_heights.size() - 1) { continue; } // Skip if on last iteration
        claw.move_velocity(+200);
        chassis.tank(70, 70, true);
        pros::Task::delay(100);
        chassis.tank(0, 0, true);
        pros::Task::delay(400);
    }
}

// Left-side starting position (relative to driver alliance station)
void aut_match_stack_left() {
    const std::vector<double> stack_heights = {950, 1250, 1700};
    const std::vector<double> release_heights = {50, 450, 750, 1250};

    const bool FORWARD = true, REVERSE = false;
    const auto linear_params = [](bool facing_forwards) { return MoveToPointParams {
        .forwards = facing_forwards, .maxSpeed = 80, .minSpeed = 18, .earlyExitRange = 3.5}; };
    const auto angular_params = [](bool facing_forwards, AngularDirection direction = AUTO) {
        return TurnToPointParams { .forwards = facing_forwards, .direction = direction,
            .maxSpeed = 80, .minSpeed = 18, .earlyExitRange = 5.5};};

    const float GOAL_X = 30.0, GOAL_Y = 16.5;
    const float LOADER_X = -12.2, LOADER_Y = -1.5;

    chassis.cancelAllMotions();
    chassis.setPose(0, 0, 0, false);
    claw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    lift.set_target_position(50);

    for (uint32_t i = 0; i < stack_heights.size(); i++) {
        new pros::Task([&](){
            lift.set_target_position(stack_heights[i]);
            pros::Task::delay(50);
            claw.move_velocity(+200);
            pros::Task::delay(200);
            uint32_t decelerate_time = pros::millis();
            for (int32_t step = 1; step <= 25; ++step) {
                pros::Task::delay_until(&decelerate_time, 10);
                claw.move_velocity(200 - step * 4);
            }
            claw.move_velocity(+100);
        }, 8UL, 8192U, "auton_claw_and_lift");
        pros::Task::delay(100);

        chassis.moveToPoint((i == 0 ? 0.0 : LOADER_X), GOAL_Y, 1100, linear_params(REVERSE));
        chassis.waitUntilDone();
        chassis.turnToPoint(GOAL_X, GOAL_Y, 900, angular_params(FORWARD));
        chassis.waitUntilDone();
        chassis.moveToPoint(GOAL_X, (GOAL_Y - i * 0.75), 1000, linear_params(FORWARD));
        chassis.waitUntilDone();

        lift.set_target_position(release_heights[i]);
        pros::Task::delay(250);
        claw.move_velocity(i == 0 ? -80 : -100);
        chassis.tank(-10, -10, true);
        pros::Task::delay(i == 0 ? 400 : 250);
        lift.set_target_position(release_heights[0]);

        chassis.moveToPoint(LOADER_X + 2.0, GOAL_Y, 1400, linear_params(REVERSE));
        chassis.waitUntilDone();
        chassis.turnToPoint(LOADER_X + 1.0, LOADER_Y, 1100, angular_params(FORWARD));
        chassis.waitUntilDone();
        chassis.moveToPoint(LOADER_X,
            i == stack_heights.size() - 1 ? LOADER_Y + 5.0 : LOADER_Y,
            1100, linear_params(FORWARD));
        chassis.waitUntilDone();

        claw.move_velocity(+200);
        chassis.tank(70, 70, true);
        pros::Task::delay(100);
        chassis.tank(0, 0, true);
        pros::Task::delay(400);
    }
}