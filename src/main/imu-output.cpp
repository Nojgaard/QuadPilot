#include <Arduino.h>
#include <IMU.h>
#include<SPI.h>

IMU imu;
Vector3f ypr;
Vector3f acceleration;
unsigned long lastMillis;
unsigned long frameMillis;

void setup() {
  Serial.begin(9600);
  imu.initialize();
  lastMillis = millis();
  frameMillis = 33; // 30 frames a second
}

void loop() {
  uint8_t success = imu.read(ypr, acceleration);
  if (success != 0) {
    return;
  }
  unsigned long currentMillis = millis();
  Serial.print(currentMillis / 1000.0f);
  Serial.print(" ");
  Serial.print(ypr.x * 180/M_PI);
  Serial.print(" ");
  Serial.print(ypr.y * 180/M_PI);
  Serial.print(" ");
  Serial.println(ypr.z * 180/M_PI);

  unsigned long dt = lastMillis - currentMillis;
  lastMillis = currentMillis;
  if (dt < frameMillis)
    delay(frameMillis - dt);
}
