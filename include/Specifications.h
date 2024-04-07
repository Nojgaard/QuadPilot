#pragma once 

struct Specifications {
    /// @brief The max RPM is computed as (battery voltage * kv).
    /// Our motors has a KV of 1000, and voltage of a single battery cell ranges from ~[3.7, 4.2] 
    /// depending on charge. We have 3 cells and assume a voltage of 3.9. 
    static const int MAX_MOTOR_RPM = 11700;
    static const int MIN_MOTOR_RPM = 0;

    /// @brief Length from center of quadcopter to motor (meters).
    static const float ARM_LENGTH = 0.29f;

    /// @brief Total mass of quadcopter (kg)
    static const float MASS = 1.0f;

    /// @brief Thrust coeffecient (kg-m)
    static const float THRUST_COEFFECIENT = 2.39e-5f;

    /// @brief Drag torque coefficients (kg-m^2/s^2)
    static const float DRAG_TORQUE_COEFFECIENT = 1.39e-6f;

    /// @brief Acceleration of gravity m/s^2
    static const float GRAVITY = 9.8006f;
};