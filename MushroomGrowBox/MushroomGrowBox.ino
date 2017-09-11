//Mushroom Grow Box V1
//Chris Loidolt 09/2017
//References:
//https://github.com/cenkaetaya/1/blob/master/networkstable.txt
/*
* Mushroom Grow Box V1
* Chris Loidolt
* 
* References:
* https://www.reddit.com/r/arduino/comments/2s68jp/arduino_grow_room_climate_control_project/
* https://github.com/cenkaetaya/1/blob/master/networkstable.txt
* 
*/

//Libraries
#include "FastLED.h"
#include "Button.h"                                           // Button library. Includes press, long press, double press detection.
#include <DHT.h>
#include <DHT_U.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>    // Core graphics library

char* myString = { "OFF" };//relay1 on / off status
char* myString2 = { "OFF" };//relay2 on / off status
const int chipSelect = 4;

//DHT22 Setup
#define DHTPIN 2 
#define DHTTYPE DHT22
#define DHT2PIN 3
#define DHTTYPE2 DHT22
#define DHT3PIN 4
#define DHTTYPE3 DHT22
DHT dht3(DHT3PIN, DHTTYPE3);
DHT dht2(DHT2PIN, DHTTYPE2);
DHT dht(DHTPIN, DHTTYPE);

// DS18B20 setup
#define ONE_WIRE_BUS A1
OneWire ds(ONE_WIRE_BUS);
DallasTemperature sensors(&ds);

//RTC Setup
RTC_DS1307 RTC;

// FastLED Setup
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN   8
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    5
#define BRIGHTNESS  10
CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND  30

//Hardware Setup
const int heat1 = ;
const int cool1 = ;
const int statusLED1 = ;
const int lightingLED1 = ;
const int enableSwitch1 =;

const int heat2 = ;
const int cool2 = ;
const int statusLED2 = ;
const int lightingLED2 = ;
const int enableSwitch2 =;

const int heat3 = ;
const int cool3 = ;
const int statusLED3 = ;
const int lightingLED3 = ;
const int enableSwitch3 =;

const int humidifier = ;
const int exhaustFan = ;

// Reference Values
float idealTemp = 95;
float warningTempLow = 55;
float dangerTempLow = 46;

float idealHumid = 55;
float warningHumidLow = 45;
float warningHumidHigh = 75;
float dangerHumidLow = 35;
float dangerHumidHigh = 85;

//Climate Variables
float temp1;
float humid1;

float temp2;
float humid2;

float temp3;
float humid3;

//Status Variables
bool enableStatus1 = true;
bool enableStatus2 = true;
bool enableStatus3 = true;

int climateStatus1 = 0;
int climateStatus2 = 0;
int climateStatus3 = 0;

int growthStatus1 = 0;
int growthStatus2 = 0;
int growthStatus3 = 0;


void setup() {

  delay(1000);
  
  //Define Hardware Pins
  pinMode(heat1, OUTPUT);
  pinMode(cool1, OUTPUT);
  pinMode(statusLED1, OUTPUT);
  pinMode(lightingLED1, OUTPUT);
  pinMode(enableSwitch1, INPUT_PULLUP);

  pinMode(heat2, OUTPUT);
  pinMode(cool2, OUTPUT);
  pinMode(statusLED2, OUTPUT);
  pinMode(lightingLED2, OUTPUT);
  pinMode(enableSwitch2, INPUT_PULLUP);

  pinMode(heat2, OUTPUT);
  pinMode(cool2, OUTPUT);
  pinMode(statusLED2, OUTPUT);
  pinMode(lightingLED2, OUTPUT);
  pinMode(enableSwitch2, INPUT_PULLUP);

  pinMode(humidifier, OUTPUT);
  pinMode(exhaustFan, OUTPUT);

  //Init FastLED
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness( BRIGHTNESS );

  //Init RTC
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
  //   RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  //Start Services
  Serial.begin(9600);
  Wire.begin();
  dht.begin();
  sensors.begin();
  
}
void loop(){ 

  //Start Real Time Clock
  startRTC();
    
  // Check sensors periodically
  const unsigned long minutes = 5 * 60 * 1000UL;
  static unsigned long lastSampleTime = 0 - minutes;  // initialize such that a reading is due the first time through loop()
  
  unsigned long now = millis();
  if (now - lastSampleTime >= minutes)
  {
    lastSampleTime += minutes;
    updateSensors();
  }

  //Check Chamber Enable Switches
  updateEnableStatus();

  //Control Climate
  

  //Update LCD
  updateLCD();

}

void updateLCD() {

  //Display Temp and Humidity
  //Chamber 1
  lcd.setCursor(0,0);
  lcd.print("1:");
  lcd.print(temp1);
  lcd.print(:);
  lcd.print(humid1);
  switch (climateStatus1) {
    case 1:
      lcd.print("H");
      break;
    case 2:
      lcd.print("C");
      break;
    case 3:
      lcd.print("W");
      break;
    case 4:
      lcd.print("E");
      break;
    default: 
      lcd.print("-");
    break;
  }
  switch (growthStatus1) {
    case 1:
      lcd.print("S");
      break;
    case 2:
      lcd.print("C");
      break;
    case 3:
      lcd.print("F");
      break;
    default: 
      lcd.print("M");
    break;
  }

  //Chamber 2
  lcd.setCursor(8,0);
  lcd.print("2:");
  lcd.print(temp2);
  lcd.print(:);
  lcd.print(humid2);
  switch (climateStatus2) {
    case 1:
      lcd.print("H");
      break;
    case 2:
      lcd.print("C");
      break;
    case 3:
      lcd.print("W");
      break;
    case 4:
      lcd.print("E");
      break;
    default: 
      lcd.print("-");
    break;
  }
  switch (growthStatus2) {
    case 1:
      lcd.print("S");
      break;
    case 2:
      lcd.print("C");
      break;
    case 3:
      lcd.print("F");
      break;
    default: 
      lcd.print("M");
    break;
  }
  
  //Chamber 3
  lcd.setCursor(0,1);
  lcd.print("3:");
  lcd.print(temp3);
  lcd.print(:);
  lcd.print(humid3);
  switch (climateStatus3) {
    case 1:
      lcd.print("H");
      break;
    case 2:
      lcd.print("C");
      break;
    case 3:
      lcd.print("W");
      break;
    case 4:
      lcd.print("E");
      break;
    default: 
      lcd.print("-");
    break;
  }
  switch (growthStatus3) {
    case 1:
      lcd.print("S");
      break;
    case 2:
      lcd.print("C");
      break;
    case 3:
      lcd.print("F");
      break;
    default: 
      lcd.print("M");
    break;
  }
  

}

void updateEnableStatus() {

  //Chamber 1
  if (enableSwitch1 == LOW){
    enableStatus1 = true;
    digitalWrite(statusLED1, HIGH);
  }
  else {
    enableStatus1 = false;
    digitalWrite(statusLED1, LOW);
  }

  //Chamber 2
  if (enableSwitch2 == LOW){
    enableStatus2 = true;
    digitalWrite(statusLED2, HIGH);
  }
  else {
    enableStatus2 = false;
    digitalWrite(statusLED2, LOW);
  }

  //Chamber 3
  if (enableSwitch3 == LOW){
    enableStatus3 = true;
    digitalWrite(statusLED3, HIGH);
  }
  else {
    enableStatus3 = false;
    digitalWrite(statusLED3, LOW);
  }
  
}

void updateSensors(){

  //Chamber 1 Sensors
  Serial.println("Sense Chamber 1");
  humid1 = dht.readHumidity();
  
  float t1A = dht.readTemperature(true);
  sensors.requestTemperatures();
  float t1B = sensors.getTempFByIndex(0);

  temp1 = (t1A + t1B) / 2;

  //Chamber 2 Sensors
  Serial.println("Sense Chamber 2");
  humid2 = dht2.readHumidity();
  
  float t2A = dht2.readTemperature(true);
  sensors.requestTemperatures();
  float t2B = sensors.getTempFByIndex(0);

  temp2 = (t2A + t2B) / 2;

  //Chamber 3 Sensors
  Serial.println("Sense Chamber 3");
  humid3 = dht3.readHumidity();
  
  float t3A = dht3.readTemperature(true);
  sensors.requestTemperatures();
  float t3B = sensors.getTempFByIndex(0);

  temp1 = (t3A + t3B) / 2;

}

void startRTC() {

  DateTime now = RTC.now();
 
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
 //Hour Conversion To 12 Hour Start
   int x;
   int timeresult;
   x = (now.hour());
   if (x > 12) {
   timeresult = x - 12;
   }
   else 
   {
    timeresult = x;
   }
   Serial.println(timeresult);

}

