#include <IMU.h>
#include <Debug.h>
#include "Wire.h"
#include <EEPROM.h>
#include <Specifications.h>

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

    // read the calibration from EEPROM
    readCalibration();

    // turn on the DMP, now that it's ready
    DEBUG_PRINTLN((F("Enabling DMP...")));
    _mpu.setDMPEnabled(true);

    lastRead = -1;

    // get expected DMP packet size for later comparison
    _packetSize = _mpu.dmpGetFIFOPacketSize();
    return true;
}

uint8_t IMU::read(Vector3f& yawPitchRoll, Vector3f& inertialFrameVelocity, float& dt) {
    // dmpGetCurrentFIFOPacket return 1 on success and 0 on failure
    uint8_t success = _mpu.dmpGetCurrentFIFOPacket(_fifoBuffer);

    if (success == 1)
        return success;
    if (lastRead < 0)
    {
        lastRead = millis();
        return 1;
    }

    dt = (millis() - lastRead)/1000.0;
    lastRead = millis();

    success = 0;
    // read yaw pitch and roll
    success += _mpu.dmpGetQuaternion(&_orientation, _fifoBuffer);
    success += _mpu.dmpGetGravity(&_gravity, &_orientation);
    success += _mpu.dmpGetYawPitchRoll(yawPitchRoll.array(), &_orientation, &_gravity);

    float tmp = yawPitchRoll.x;
    yawPitchRoll.x = yawPitchRoll.z;
    yawPitchRoll.z = tmp;

    // read acceleration in inertial frame
    success += _mpu.dmpGetAccel(&_accelerationSensor, _fifoBuffer);
    success += _mpu.dmpGetLinearAccel(&_accelerationReal, &_accelerationSensor, &_gravity);
    success += _mpu.dmpConvertToWorldFrame(&_inertialFrameAcceleration, &_accelerationReal, &_orientation);

    inertialFrameVelocity.x += (int)(100 * dt * (_inertialFrameAcceleration.x / ACC_SCALE * Specifications::GRAVITY) + 0.5) / 100.0;
    inertialFrameVelocity.y += (int)(100 * dt * (_inertialFrameAcceleration.y / ACC_SCALE * Specifications::GRAVITY) + 0.5) / 100.0;
    inertialFrameVelocity.z += (int)(100 * dt * (_inertialFrameAcceleration.z / ACC_SCALE * Specifications::GRAVITY)+ 0.5) / 100.0;

    return success;
}

void IMU::calibrate() {
    Serial.println("Old active offset:");
    _mpu.PrintActiveOffsets();
    // Calibration Time: generate offsets and calibrate our MPU6050
    Serial.println("Calibrating acceleration offsets...");
    _mpu.CalibrateAccel(10);
    Serial.println("");
    Serial.println("Calibrating gyro offsets...");
    _mpu.CalibrateGyro(10);
    Serial.println("");
    Serial.println("Done! Found new offsets:");
    _mpu.PrintActiveOffsets();
    Serial.println("Storing offsets on EEPROM...");
    storeCalibration();
    Serial.println("Calibration finished!");
}

void IMU::storeCalibration() {
    int address = 0;
    
    EEPROM.put(address + sizeof(int16_t) * 0, _mpu.getXAccelOffset());
    EEPROM.put(address + sizeof(int16_t) * 1, _mpu.getYAccelOffset());
    EEPROM.put(address + sizeof(int16_t) * 2, _mpu.getZAccelOffset());

    EEPROM.put(address + sizeof(int16_t) * 3, _mpu.getXGyroOffset());
    EEPROM.put(address + sizeof(int16_t) * 4, _mpu.getYGyroOffset());
    EEPROM.put(address + sizeof(int16_t) * 5, _mpu.getZGyroOffset());
}

void IMU::readCalibration() {
    int address = 0;
    int16_t data = 0;

    _mpu.setXAccelOffset(EEPROM.get(address + sizeof(int16_t) * 0, data));
    _mpu.setYAccelOffset(EEPROM.get(address + sizeof(int16_t) * 1, data));
    _mpu.setZAccelOffset(EEPROM.get(address + sizeof(int16_t) * 2, data));

    _mpu.setXGyroOffset(EEPROM.get(address + sizeof(int16_t) * 3, data));
    _mpu.setYGyroOffset(EEPROM.get(address + sizeof(int16_t) * 4, data));
    _mpu.setZGyroOffset(EEPROM.get(address + sizeof(int16_t) * 5, data));
}