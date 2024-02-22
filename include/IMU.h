#pragma once

#include "MPU6050_6Axis_MotionApps20.h"

class IMU {
    public:
    bool initialize();

    private:
    MPU6050 _mpu;

    // MPU control/status vars
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer
};