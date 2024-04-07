#include <Arduino.h>
#include <IMU.h>
#include <MotorController.h>
#include <Controller.h>
#include <PID.h>
#include <Vector3.h>
#include <RF24.h>

IMU imu;
MotorController motors;
Controller controller;

PID pidAttitude(.8f, 1.0f, 0.4f, 0.1f);
PID pidVelocities(.8f, 1.0f, 0.4f, 0.1f);

Vector3f targetAttitude;
Vector3f targetVelocity;
Vector3f measuredAttitude;
Vector3f measuredVelocity;

long targetRPMSqr[Specifications::NUM_MOTORS] = {0, 0, 0, 0};

long lastMillis;

int MOTOR_PINS[Specifications::NUM_MOTORS] = {10, 9, 5, 4};

void setup() {
    imu.initialize();
    motors.initialize(MOTOR_PINS);
    delay(5000);
    motors.setRPM(targetRPMSqr);
}

void loop() {
    long currentMillis = millis();
    float dt = (currentMillis - lastMillis) / 1000.0f; // delta time in seconds
    lastMillis = currentMillis;

    imu.read(measuredAttitude, measuredVelocity);
    pidAttitude.update(targetAttitude, measuredAttitude, dt);
    pidVelocities.update(targetVelocity, measuredVelocity, dt);

    controller.update(measuredAttitude, pidAttitude, pidVelocities, targetRPMSqr);
    motors.setRPM(targetRPMSqr);   
}