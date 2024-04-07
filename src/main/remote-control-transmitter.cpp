#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(5, 6); // CE, CSN

const byte address[6] = "00001";
const int POTENTIOMETER_PIN = 7;
const int BUTTON_PIN = 2;


struct Copter_Control {
  bool powerOn = 0;
  int motorSpeed = 0;

  Copter_Control(bool powerOn, int motorSpeed){
    this->powerOn = powerOn;
    this->motorSpeed = motorSpeed;
  }
};

void setup() {

  Serial.begin(9600);

 pinMode(BUTTON_PIN, INPUT_PULLUP);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  //const char text[] = "Hello World";
  //radio.write(&text, sizeof(text));
  //delay(1000);

  int motorSpeed = analogRead(POTENTIOMETER_PIN);
  motorSpeed = map(motorSpeed, 0, 1024, 0, 100);
   //Serial.println(motorSpeed);

  bool powerOn = digitalRead(BUTTON_PIN);
  Serial.println(powerOn);

  auto package = Copter_Control(powerOn, motorSpeed);
  radio.write(&package, sizeof(Copter_Control));

  delay(10);
}