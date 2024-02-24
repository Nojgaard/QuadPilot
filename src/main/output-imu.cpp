#include <Arduino.h>
#include <IMU.h>

IMU imu;
Vector3f ypr;
Vector3i16 acceleration;

void setup() {
  Serial.begin(9600);
  imu.initialize();
}

void loop() {
  uint8_t success = imu.read(ypr, acceleration);
  if (success != 0) {
    return;
  }
  Serial.print("ypr\t");
  Serial.print(ypr.x * 180/M_PI);
  Serial.print("\t");
  Serial.print(ypr.y * 180/M_PI);
  Serial.print("\t");
  Serial.println(ypr.z * 180/M_PI);
}
