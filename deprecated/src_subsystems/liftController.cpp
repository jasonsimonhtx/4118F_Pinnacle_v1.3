#include "main.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <limits>

LiftController::LiftController(pros::MotorGroup* motors, pros::Rotation* rotation_sensor,
    const LiftMotionConfig& motion, const LiftGeometry& geometry)
    : m_motors(motors), m_rotation_sensor(rotation_sensor), m_motion(motion), m_geometry(geometry),
      m_state(LiftState::IDLE), m_target_pos(m_geometry.min_degrees), m_prev_volts(0.0),
      m_target_vel(0.0), m_position_error_reported(false), m_velocity_error_reported(false),
      m_periodic_task(nullptr)
{
}

void LiftController::initialize() {
    if (m_periodic_task != nullptr) {
        return;
    }

    initialize_devices();
    m_periodic_task = new pros::Task(periodic_task_trampoline, this,
        9UL, 8192U, "lift_periodic");
}

LiftController::~LiftController() {
    if (m_periodic_task != nullptr) {
        m_periodic_task->remove();
        delete m_periodic_task;
    }
}

void LiftController::periodic_task_trampoline(void* param) {
    LiftController* instance = static_cast<LiftController*>(param);
    instance->periodic();
}

LiftState LiftController::get_state() {
    return m_state;
}

void LiftController::set_voltage(double volts, double current_position) {
    if (m_motors == nullptr) { return; }
    if (!std::isfinite(current_position)) {
        volts = 0.0;
    } else if ((current_position <= m_geometry.min_degrees && volts < 0.0) ||
               (current_position >= m_geometry.max_degrees && volts > 0.0)) {
        volts = 0.0;
    }
    volts = std::clamp(volts, -m_motion.max_voltage, m_motion.max_voltage);
    m_prev_volts = volts;
    m_motors->move_voltage(static_cast<int32_t>(volts * 1000.0));
}

double LiftController::get_current_position() {
    if (m_rotation_sensor == nullptr) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    const std::int32_t position = m_rotation_sensor->get_position();
    if (position == PROS_ERR) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return position / 100.0;
}

double LiftController::get_current_velocity() {
    if (m_rotation_sensor == nullptr) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    const std::int32_t velocity = m_rotation_sensor->get_velocity();
    if (velocity == PROS_ERR) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return static_cast<double>(velocity);
}

void LiftController::set_target_position(double target_pos) {
    m_state = LiftState::POSITIONING;
    m_target_pos = std::clamp(target_pos, m_geometry.min_degrees, m_geometry.max_degrees);
}

double LiftController::get_target_position() {
    return m_target_pos;
}

void LiftController::set_target_velocity(double target_vel) {
    m_state = LiftState::MANUAL;
    m_target_vel = std::clamp(target_vel, -600.0, 600.0);
}

double LiftController::get_target_velocity() {
    return m_target_vel;
}

void LiftController::initialize_devices() {
    if (m_motors != nullptr) {
        m_motors->set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
        m_motors->set_current_limit_all(2500);
        m_motors->set_encoder_units_all(pros::E_MOTOR_ENCODER_DEGREES);
        m_motors->set_voltage_limit_all(static_cast<int32_t>(m_motion.max_voltage * 1000.0));
    }
    
    if (m_rotation_sensor != nullptr) {
        m_rotation_sensor->set_data_rate(10);
    }

    pros::Task::delay(10); // Allow the sensor reading to refresh
    m_state = LiftState::IDLE;
}

void LiftController::periodic() {
    uint32_t _millis = pros::millis();
    const uint32_t _delta = m_motion.delta;

    while (true) {
        double current_position = get_current_position();

        if (!std::isfinite(current_position)) {
            if (!m_position_error_reported) {
                std::printf("Lift rotation sensor position unavailable; stopping lift\n");
                m_position_error_reported = true;
            }
            set_voltage(0.0, current_position);
            pros::Task::delay_until(&_millis, _delta);
            continue;
        }
        m_position_error_reported = false;

        double requested_volts = 0.0;
        if (m_state == LiftState::POSITIONING) {
            const double error = m_target_pos - current_position;
            if (std::abs(error) > 15.0) {
                const double current_velocity = get_current_velocity();
                if (!std::isfinite(current_velocity)) {
                    if (!m_velocity_error_reported) {
                        std::printf("Lift rotation sensor velocity unavailable; stopping lift\n");
                        m_velocity_error_reported = true;
                    }
                    set_voltage(0.0, current_position);
                    pros::Task::delay_until(&_millis, _delta);
                    continue;
                }
                m_velocity_error_reported = false;
                requested_volts = error * m_motion.kP + m_motion.kG
                    - m_motion.kD * current_velocity;
            }
        } else if (m_state == LiftState::MANUAL) {
            requested_volts = (m_target_vel / 600.0) * m_motion.max_voltage;
        }

        const double voltage_change = std::clamp(
            requested_volts - m_prev_volts, -m_motion.slew, m_motion.slew);
        const double volts = std::clamp(
            m_prev_volts + voltage_change, -m_motion.max_voltage, m_motion.max_voltage);
        set_voltage(volts, current_position);

        pros::Task::delay_until(&_millis, _delta);
    }
}