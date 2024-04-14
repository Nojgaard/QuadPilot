#include <MotorController.h>
#include <Arduino.h>

void MotorController::initialize(const int pins[]) {
    for (int i = 0; i < Specifications::NUM_MOTORS; i++)
        _motors->attach(pins[i], MIN_PULSE_LENGTH, MAX_PULSE_WIDTH);
}

void MotorController::setRPS(const long rpmSqr[]) {
    for (int i = 0; i < Specifications::NUM_MOTORS; i++) {
        int pulseLength = map(rpmSqr[i], Specifications::MIN_MOTOR_RPS_SQR, Specifications::MAX_MOTOR_RPS_SQR, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
        Serial.print(pulseLength);
                Serial.print(" ");
        _motors[i].writeMicroseconds(pulseLength);
    }
}