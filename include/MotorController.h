#pragma once
#include <Servo.h>
#include <Specifications.h>

class MotorController {
    public:
    static const int MIN_PULSE_LENGTH = 1015; // Minimum pulse length in µs
    static const int MAX_PULSE_LENGTH = 1400; // Maximum pulse length in µs

    void initialize(const int pins[]);
    void setRPM(const long rpmSqr[]);
    
    private:
    Servo _motors[Specifications::NUM_MOTORS];
};