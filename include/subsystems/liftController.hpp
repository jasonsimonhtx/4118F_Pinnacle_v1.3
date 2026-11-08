#pragma once
#include "main.h"

// structs + enums //

struct LiftMotionConfig {
    const double kP;           // Position proportional gain (V/degree)
    const double kD;           // Velocity damping gain (V per degree/second)
    const double kG;           // Gravity compensation voltage
    const double slew;         // Maximum voltage change per control cycle
    const double max_voltage;  // Maximum voltage output

    const uint32_t delta;      // Encoder update rate
};

struct LiftGeometry {
    const double min_degrees;       // Minimum possible position (just above ground)
    const double max_degrees;       // Maximum possible position (mechanical ceiling)
};

enum class LiftState : uint32_t {
    IDLE        = 10,
    POSITIONING = 20,
    MANUAL      = 30
};

// class declaration //

class LiftController {
private:
    pros::MotorGroup* m_motors;
    pros::Rotation* m_rotation_sensor;

    LiftMotionConfig m_motion;
    LiftGeometry m_geometry;
    LiftState m_state;

    double m_target_pos;
    double m_prev_volts;
    double m_target_vel;
    bool m_position_error_reported;
    bool m_velocity_error_reported;

    pros::Task* m_periodic_task;
    void periodic();
    static void periodic_task_trampoline(void* param);
    void set_voltage(double volts, double current_position);
    void initialize_devices();
public:
    LiftController(pros::MotorGroup* motors, pros::Rotation* rotation_sensor,
        const LiftMotionConfig& motion, const LiftGeometry& geometry);
    ~LiftController();

    LiftState get_state();

    void set_target_velocity(double target_velocity);
    double get_target_velocity();

    double get_current_position();
    double get_current_velocity();
    void set_target_position(double target_pos);
    double get_target_position();
    
    void initialize();
};