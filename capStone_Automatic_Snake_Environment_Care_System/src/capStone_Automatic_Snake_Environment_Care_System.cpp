/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

#define Grove_Water_Sensor D7
int water;
int currentTime;
int last150millisSec;

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// setup() runs once, when the device is first turned on
void setup() {
  pinMode(Grove_Water_Sensor,INPUT);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
   water=digitalRead(Grove_Water_Sensor);
 currentTime = millis ();
  if((currentTime-last150millisSec)>150) {
    last150millisSec = millis();
 Serial.printf("water %i ", water);
}
}
