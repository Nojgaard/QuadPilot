#include <Arduino.h>
#include <Servo.h>

/**
 * Usage, according to documentation(https://www.firediy.fr/files/drone/HW-01-V4.pdf) : 
 *     1. Plug your Arduino to your computer with USB cable, open terminal, then type 1 to send max throttle to every ESC to enter programming mode
 *     2. Power up your ESCs. You must hear "beep1 beep2 beep3" tones meaning the power supply is OK
 *     3. After 2sec, "beep beep" tone emits, meaning the throttle highest point has been correctly confirmed
 *     4. Type 0 to send min throttle
 *     5. Several "beep" tones emits, which means the quantity of the lithium battery cells (3 beeps for a 3 cells LiPo)
 *     6. A long beep tone emits meaning the throttle lowest point has been correctly confirmed
 *     7. Type 2 to launch test function. This will send min to max throttle to ESCs to test them
 */
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Customize here pulse lengths as needed
#define MIN_PULSE_LENGTH 1015 // Minimum pulse length in µs
#define MAX_PULSE_LENGTH 1400 // Maximum pulse length in µs
// for our motors it seems to be 1015 - 1400
// ---------------------------------------------------------------------------
Servo motA, motB, motC, motD;
char data;
// ---------------------------------------------------------------------------


int potValue;  // value from the analog pin

/**
 * Initialisation routine
 */
void setup() {
    Serial.begin(9600);
    motA.attach(10, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    motB.attach(9, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    delay(5000); // wait 5 sec
}

/**
 * Main function
 */
void loop() {
  potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  potValue = map(potValue, 0, 1023, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);   // scale it to use it with the servo library 
  Serial.println(potValue);
  motA.writeMicroseconds(potValue);    // Send the signal to the ESC
  motB.writeMicroseconds(potValue);    // Send the signal to the ESC
}
