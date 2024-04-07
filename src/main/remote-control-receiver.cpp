#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

struct Copter_Control {
  bool powerOn = 0;
  int motorSpeed = 0;
};

Copter_Control data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
        radio.read(&data, sizeof(Copter_Control));

        Serial.println(data.powerOn);
        Serial.println(data.motorSpeed);
  }
}