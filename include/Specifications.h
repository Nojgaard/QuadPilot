#pragma once 

#include <Arduino.h>

namespace Specifications {
    static const short NUM_MOTORS = 4;

    /// @brief The max/min revolutions-per-minute (RPM) is computed as (battery voltage * kv).
    /// Our motors has a KV of 1000, and voltage of a single battery cell ranges from ~[3.7, 4.2] 
    /// depending on charge. We have 3 cells and assume a voltage of 3.9. 
    static const long MAX_MOTOR_RPM = 11700;
    static const long MIN_MOTOR_RPM = 0;

    static const float RPM_TO_RPS = 2*PI / 60.0;

    static const long MAX_MOTOR_RPS_SQR = RPM_TO_RPS * RPM_TO_RPS * MAX_MOTOR_RPM * MAX_MOTOR_RPM;
    static const long MIN_MOTOR_RPS_SQR = RPM_TO_RPS * RPM_TO_RPS * MIN_MOTOR_RPM * MIN_MOTOR_RPM;

    /// @brief Length from center of quadcopter to motor (meters).
    static const float ARM_LENGTH = 0.29f;

    /// @brief Total mass of quadcopter (kg)
    static const float MASS = 1.0f;

    /// @brief Thrust coeffecient (kg-m)
    static const float THRUST_COEFFECIENT = 2.39e-6f;

    /// @brief Drag torque coefficients (kg-m^2/s^2)
    static const float DRAG_TORQUE_COEFFECIENT = 1.39e-6f;

    /// @brief Acceleration of gravity m/s^2
    static const float GRAVITY = 9.8006f;
};