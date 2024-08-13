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
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"

///************ Global State (you don't need to change this!) ***   ***************/ 
  TCPClient TheClient; 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

///****************************** Feeds ***************************************/ 
// Setup Feeds to publish or subscribe 
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> 
Adafruit_MQTT_Subscribe capStone = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/capStone"); 
Adafruit_MQTT_Publish capHumidityfeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/capHumidityfeed");
Adafruit_MQTT_Publish capMoisturefeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/capMoisturefeed");
Adafruit_MQTT_Publish capTempFfeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/capTempFfeed");
Adafruit_MQTT_Publish capWaterSensorfeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/capWaterSensorfeed");
//this is the header, it defines variables which are used in the code.
const int Grove_Water_Sensor = S2;
const int OLED_RESET=-1;
const int hexAddress = 0x76;
Adafruit_BME280 bme ;
Adafruit_SSD1306 display(OLED_RESET);
int randNumber;
int subValue;
int water;
int currentTime;
int rot;
int last150MillisSec;
int last50MillisSec;
int moisture;
int lastTempF;
unsigned int last, lastTime;
const int straightWater = 1530;
const int straightair = 3585;
const int moistSoil = 2800;
const int drySoil = 3200;
float tempC;
float humidity;
float tempF;
float numValue;
bool status;

/************Declare Functions*************/
void MQTT_connect();
bool MQTT_ping();

// setup() runs once, when the device is first turned on
void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected,10000);

  Serial.printf("\n\n");

  pinMode(Grove_Water_Sensor,INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(S1, OUTPUT);
  pinMode(D7, OUTPUT);

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
 
 //Setup MQTT subscription
 mqtt.subscribe(&capStone);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  MQTT_connect();
  MQTT_ping();

  // this is our 'wait for incoming subscription packets' busy subloop 
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100))) {
    if (subscription == &capStone) {
      subValue = atoi((char *)capStone.lastread);
    }
    Serial.printf("button %i\n",subValue);
  }

  if((millis()-lastTime > 8000)) {
    if(mqtt.Update()) {
      capHumidityfeed.publish(humidity);
      Serial.printf("Publishing humidity %0.2f\n",capHumidityfeed);
    }
    lastTime = millis();
  }

  if((millis()-lastTime > 8000)) {
    if(mqtt.Update()) {
      capTempFfeed.publish(tempF);
      Serial.printf("Publishing %0.2f\n",capTempFfeed); 
      } 
    lastTime = millis();
  }

  if((millis()-lastTime > 55000)) {
    if(mqtt.Update()) {
      capMoisturefeed.publish(moisture);
      Serial.printf("Publishing moisture %0.2f\n",capMoisturefeed);
    }
    lastTime = millis();
  }

  if((millis()-lastTime > 55000)) {
    if(mqtt.Update()) {
      capWaterSensorfeed.publish(water);
      Serial.printf("Publishing WaterSensor %i\n",capWaterSensorfeed);
    }
    lastTime = millis();
  }
  
 water=digitalRead(Grove_Water_Sensor);

 moisture=analogRead(A0);

 digitalWrite(S1,HIGH);
 digitalWrite(D7,HIGH);

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


//Function to connect and reconnect as necessary to the MQTT server.
//Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
 
  // Return if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.printf("Error Code %s\n",mqtt.connectErrorString(ret));
       Serial.printf("Retrying MQTT connection in 5 seconds...\n");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds and try again
  }
  Serial.printf("MQTT Connected!\n");
}

bool MQTT_ping() {
  static unsigned int last;
  bool pingStatus;

  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      pingStatus = mqtt.ping();
      if(!pingStatus) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
  return pingStatus;
}