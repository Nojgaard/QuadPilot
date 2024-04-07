#include <MotorController.h>
#include <Arduino.h>

void MotorController::initialize(const int pins[]) {
    for (int i = 0; i < Specifications::NUM_MOTORS; i++)
        _motors->attach(pins[i], MIN_PULSE_LENGTH, MAX_PULSE_WIDTH);
}

void MotorController::setRPM(const long rpmSqr[]) {
    for (int i = 0; i < Specifications::NUM_MOTORS; i++) {
        int pulseLength = map(rpmSqr[i], Specifications::MIN_MOTOR_RPM_SQR, Specifications::MAX_MOTOR_RPM_SQR, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
        _motors[i].writeMicroseconds(pulseLength);
    }
}