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
#include "Adafruit_BME280.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "credentials.h"

//this is the header, it defines variables which are used in the code.
#define Grove_Water_Sensor D8
const int OLED_RESET=-1;
const int hexAddress = 0x76;
Adafruit_BME280 bme ;
Adafruit_SSD1306 display(OLED_RESET);
int water;
int currentTime;
int last150MillisSec;
int last50MillisSec;
int moisture;
const int straightWater = 1530;
const int straightair = 3585;
const int moistSoil = 2800;
const int drySoil = 3200;
float tempC;
float humidity;
float tempF;
bool status;



// setup() runs once, when the device is first turned on
void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected,10000);

  Serial.printf("\n\n");

  pinMode(Grove_Water_Sensor,INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  status = bme . begin (hexAddress) ;
  if(status == false) {
    Serial.printf("BME280 at address 0x%02X failed to start", hexAddress);
 }

  pinMode(A0,INPUT);

  display.display();
  delay(1000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.printf("tempF %0.2f\n,humidity %0.2f\n water %i\n moisture %i ",tempF,humidity,water,moisture);
  display.display();
  display.clearDisplay();

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
 water=digitalRead(Grove_Water_Sensor);

 moisture=analogRead(A0);

currentTime = millis ();
if((currentTime-last150MillisSec)>150) {
  last150MillisSec = millis();
tempC = bme.readTemperature();
  tempF = 1.8 * tempC + 32;
humidity = bme.readHumidity();
  Serial.printf("tempF %0.2f\n,humidity %0.2f\n water %i\n moisture %i ",tempF,humidity,water,moisture);
}

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.printf("tempF %0.2f\n,humidity %0.2f\n water %i\n moisture %i ",tempF,humidity,water,moisture);
  display.display();
  display.clearDisplay();

}
