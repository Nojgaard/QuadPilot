#pragma once

#include "MPU6050_6Axis_MotionApps20.h"
#include <Vector3.h>

/**
 * Inertial Measuring Unit (IMU) used to retrieve the euler angles and acceleration
 * of the quadcopter during flight.
 * 
 * The IMU uses the Digital Motion Processor (DMP) onboard the Motion Processing Unit (MPU)
 * MPU6050 to obtain accurate readings.
*/
class IMU {
    public:
    bool initialize();
    uint8_t read(Vector3f& yawPitchRoll, Vector3i16& inertialFrameAcceleration);
    void calibrate();

    private:
    void storeCalibration();
    void readCalibration();

    MPU6050 _mpu;

    // MPU control/status vars
    uint16_t _packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t _fifoCount;     // count of all bytes currently in FIFO
    uint8_t _fifoBuffer[64]; // FIFO storage buffer

    Quaternion _orientation;           // [w, x, y, z]         quaternion container
    VectorInt16 _accelerationSensor;   // [x, y, z]            accel sensor measurements
    VectorInt16 _accelerationReal;     // [x, y, z]            gravity-free accel sensor measurements
    VectorFloat _gravity;              // [x, y, z]            gravity vector
};