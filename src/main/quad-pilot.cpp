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

float dt;

long targetRPSSqr[Specifications::NUM_MOTORS] = {0, 0, 0, 0};

long lastMillis;

int MOTOR_PINS[Specifications::NUM_MOTORS] = {10, 9, 5, 6};

void setup() {
    Serial.begin(9600);
    
    imu.initialize();
    imu.calibrate();
    //motors.initialize(MOTOR_PINS);
    //delay(5000);
    //motors.setRPS(targetRPMSqr);
}

void loop() {

    auto success = imu.read(measuredAttitude, measuredVelocity, dt);
    
    if (success != 0){
        return;
    }

    Serial.print(" ");
    Serial.print(measuredAttitude.x);
    Serial.print(" ");
    Serial.print(measuredAttitude.y);
    Serial.print(" ");
    Serial.println(measuredAttitude.z);

    pidAttitude.update(targetAttitude, measuredAttitude, dt);
    pidVelocities.update(targetVelocity, measuredVelocity, dt);

    controller.update(measuredAttitude, pidAttitude, pidVelocities, targetRPSSqr);
    
    motors.setRPS(targetRPSSqr);
    Serial.println(" ");

    delay(100);

    //motors.setRPS(targetRPMSqr);

}