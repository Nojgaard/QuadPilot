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

long targetRPMSqr[Specifications::NUM_MOTORS] = {0, 0, 0, 0};

void setup() {
    
    Serial.begin(9600);
    imu.initialize();
    //imu.calibrate();
}

void loop() {
    uint8_t success = imu.read(measuredAttitude, measuredVelocity);

    if (success != 0) {
        return;
    }

    if (leastReadMillis < 0){
        leastReadMillis = millis();
        return;
    }


    long currentMillis = millis();
    float dt = (currentMillis - leastReadMillis) / 1000.0f; // delta time in seconds
    leastReadMillis = currentMillis;
    pidAttitude.update(targetAttitude, measuredAttitude, dt);
    controller.update(measuredAttitude, pidAttitude, pidVelocities, targetRPMSqr);

    
    Serial.print(currentMillis / 1000.0f); Serial.print(" ");

    for (int i = 0; i < Specifications::NUM_MOTORS; i++) {
        long percMotorThrust = map(targetRPMSqr[i], Specifications::MIN_MOTOR_RPM_SQR, Specifications::MAX_MOTOR_RPM_SQR, 0, 100);
        Serial.print(percMotorThrust); Serial.print(" ");
    }

    Serial.println();

    delay(30);
}