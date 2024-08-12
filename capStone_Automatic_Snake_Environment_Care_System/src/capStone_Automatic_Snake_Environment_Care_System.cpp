/* 
 * Project capStone_Automatic_Snake_Environment_Care_System
 * Description: This my capstone for the class, it's a system to take care of my snakes environment when i am not available.
 * Author: Johnathan
 * Date: 8/12/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

//this is the header, it defines variables which are used in the code.
#define Grove_Water_Sensor D7
int water;
int currentTime;
int last150MillisSec;

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
  if((currentTime-last150MillisSec)>150) {
    last150MillisSec = millis();
 Serial.printf("water %i ", water);
}
}
