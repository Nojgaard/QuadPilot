#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Customize here pulse lengths as needed
#define MIN_PULSE_LENGTH 1015 // Minimum pulse length in µs
#define MAX_PULSE_LENGTH 1400 // Maximum pulse length in µs
// for our motors it seems to be 1015 - 1400
// ---------------------------------------------------------------------------
Servo motA, motB, motC, motD;

int motorSpeed = MIN_PULSE_LENGTH;

unsigned long deadMandSwitchTimeMS = 400;
unsigned long lastReceivedSignalMs = 0;
bool terminate = false;


RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

struct Copter_Control {
  bool powerOn = 0;
  int motorSpeed = 0;
};

Copter_Control data;

void setup(){

    Serial.begin(9600);

    motA.attach(10, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    motB.attach(9, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    motC.attach(5, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    motD.attach(6, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    delay(5000); // wait 5 sec

    // hook up NRF
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    motA.writeMicroseconds(MIN_PULSE_LENGTH);
    motB.writeMicroseconds(MIN_PULSE_LENGTH); 
    motC.writeMicroseconds(MIN_PULSE_LENGTH); 

    motD.writeMicroseconds(MIN_PULSE_LENGTH); 

    lastReceivedSignalMs = millis();

}

void loop() {

    if (terminate){
        return;
    }

    //Serial.println((millis() - lastReceivedSignalMs));
    if((millis() - lastReceivedSignalMs) >  deadMandSwitchTimeMS)
    {   
        Serial.println("Shuting down");  
        motA.writeMicroseconds(MIN_PULSE_LENGTH);
        motB.writeMicroseconds(MIN_PULSE_LENGTH); 
        motC.writeMicroseconds(MIN_PULSE_LENGTH); 
        motD.writeMicroseconds(MIN_PULSE_LENGTH); 
        motA.detach();
        motB.detach();
        motC.detach();
        motD.detach();

        terminate = true;
        return;
    }

    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    if (radio.available()) {
        lastReceivedSignalMs = millis();

        radio.read(&data, sizeof(Copter_Control));

        if(data.powerOn == false){
            Serial.println("power off");
            motA.writeMicroseconds(MIN_PULSE_LENGTH);
            motB.writeMicroseconds(MIN_PULSE_LENGTH);
            motC.writeMicroseconds(MIN_PULSE_LENGTH);
            motD.writeMicroseconds(MIN_PULSE_LENGTH);
        }
        else{
            motorSpeed = map(data.motorSpeed, 0, 100, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
            Serial.println(motorSpeed);
            motA.writeMicroseconds(motorSpeed);
            motB.writeMicroseconds(motorSpeed);
            motC.writeMicroseconds(motorSpeed);
            motD.writeMicroseconds(motorSpeed);
        }
    }
}