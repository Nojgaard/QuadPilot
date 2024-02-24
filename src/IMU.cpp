#include <IMU.h>
#include <Debug.h>
#include "Wire.h"

bool IMU::initialize() {
    // Exit if Serial has not initialized
    if (!Serial) return false;

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

    // initialize device
    DEBUG_PRINTLN(F("Initializing I2C devices..."));
    _mpu.initialize();

    // verify connection
    DEBUG_PRINTLN(F("Testing device connections..."));
    DEBUG_PRINTLN(_mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    DEBUG_PRINTLN(F("Initializing DMP..."));
    int deviceStatus = _mpu.dmpInitialize(); // 0 if successful

    if (deviceStatus != 0){
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        DEBUG_PRINT("DMP Initialization failed (code ");
        DEBUG_PRINT(deviceStatus);
        DEBUG_PRINTLN(")");
    }

    // Calibration Time: generate offsets and calibrate our MPU6050
    _mpu.CalibrateAccel(6);
    _mpu.CalibrateGyro(6);
    _mpu.PrintActiveOffsets();

    // turn on the DMP, now that it's ready
    DEBUG_PRINTLN((F("Enabling DMP...")));
    _mpu.setDMPEnabled(true);

    // get expected DMP packet size for later comparison
    _packetSize = _mpu.dmpGetFIFOPacketSize();
    return true;
}

uint8_t IMU::read(Vector3f& yawPitchRoll, Vector3i16& inertialFrameAcceleration) {
    // dmpGetCurrentFIFOPacket return 1 on success and 0 on failure
    uint8_t success = _mpu.dmpGetCurrentFIFOPacket(_fifoBuffer);

    if (success == 1)
        return success;
    success = 0;

    // read yaw pitch and roll
    success += _mpu.dmpGetQuaternion(&_orientation, _fifoBuffer);
    success += _mpu.dmpGetGravity(&_gravity, &_orientation);
    success += _mpu.dmpGetYawPitchRoll(yawPitchRoll.array(), &_orientation, &_gravity);

    // read acceleration in inertial frame
    success += _mpu.dmpGetAccel(&_accelerationSensor, _fifoBuffer);
    success += _mpu.dmpGetLinearAccel(&_accelerationReal, &_accelerationSensor, &_gravity);
    success += _mpu.dmpConvertToWorldFrame(&inertialFrameAcceleration, &_accelerationReal, &_orientation);

    return success;
}