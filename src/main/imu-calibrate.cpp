#include <Arduino.h>
#include <IMU.h>
#include<SPI.h>

IMU imu;

void setup() {
    Serial.begin(9600);
    Serial.println("Initializing IMU");
    imu.initialize();
    imu.calibrate();
}

void loop() {

}