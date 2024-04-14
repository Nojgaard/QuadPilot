#include <Arduino.h>
#include <SPI.h>
#include <IMU.h>
#include <PID.h>
#include <Controller.h>
#include <Specifications.h>

IMU imu;
Controller controller;
PID pidAttitude(.8f, 1.0f, 0.4f, 0.1f);
PID pidVelocities(.8f, 1.0f, 0.4f, 0.1f);

Vector3f targetAttitude;
Vector3f targetVelocity;
Vector3f measuredAttitude;
Vector3f measuredVelocity;

long leastReadMillis = -1;
float dt;

long targetRPMSqr[Specifications::NUM_MOTORS] = {0, 0, 0, 0};

void setup() {
    
    Serial.begin(9600);
    imu.initialize();
    //imu.calibrate();
}

void loop() {
    uint8_t success = imu.read(measuredAttitude, measuredVelocity, dt);

    if (success != 0) {
        return;
    }


    long currentMillis = millis();
    pidAttitude.update(targetAttitude, measuredAttitude, dt);
    controller.update(measuredAttitude, pidAttitude, pidVelocities, targetRPMSqr);

    
    Serial.print(currentMillis / 1000.0f); Serial.print(" ");

    for (int i = 0; i < Specifications::NUM_MOTORS; i++) {
        long percMotorThrust = map(targetRPMSqr[i], Specifications::MIN_MOTOR_RPS_SQR, Specifications::MAX_MOTOR_RPS_SQR, 0, 100);
        Serial.print(percMotorThrust); Serial.print(" ");
    }

    Serial.println();

    delay(30);
}