#pragma once
#include "main.h"

/**
 * @brief A wrapper for the PROS Imu class to handle scalar errors.
 * 
 * If your robot rotates 3600 degrees but the IMU only reports 3580,
 * your scalar is 3600 / 3580.
 */
class ScaledIMU : public pros::Imu {
    public:
        /**
         * @param port The V5 port the IMU is plugged into
         * @param scalar The multiplier to apply to the rotation data
         */
        ScaledIMU(const std::uint8_t port, double scalar) 
            : pros::Imu(port), m_port(port), m_scalar(scalar) {}

        /**
         * @brief Returns the scaled rotation of the IMU
         * Overrides the base pros::Imu::get_rotation to apply the scalar.
         */
        virtual double get_rotation() const override {
            return pros::c::imu_get_rotation(m_port) * m_scalar;
        }

    private:
        const std::uint8_t m_port;
        const double m_scalar;
};