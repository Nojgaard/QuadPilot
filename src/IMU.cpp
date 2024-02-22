#include <IMU.h>
#include "Wire.h"

bool IMU::initialize() {
    // Exit if Serial has not initialized
    if (!Serial) return false;

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    _mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(_mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    int deviceStatus = _mpu.dmpInitialize(); // 0 if successful

    if (deviceStatus != 0){
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(deviceStatus);
        Serial.println(F(")"));
    }

    // Calibration Time: generate offsets and calibrate our MPU6050
    _mpu.CalibrateAccel(6);
    _mpu.CalibrateGyro(6);
    _mpu.PrintActiveOffsets();

    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    _mpu.setDMPEnabled(true);

    // get expected DMP packet size for later comparison
    packetSize = _mpu.dmpGetFIFOPacketSize();
    return true;
}