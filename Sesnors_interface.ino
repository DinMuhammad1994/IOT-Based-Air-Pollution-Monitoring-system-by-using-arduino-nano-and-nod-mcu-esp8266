#include <LiquidCrystal_I2C.h>  // lcd library

#include <ArduinoJson.h>             
#include <SoftwareSerial.h>

// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(2, 3); // RX, TX


              
#include "DHT.h"  // temperature sensor library 
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);


#define DHTPIN 13     // what digital pin we're connected to arduino nano that is pin # 13
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// 6 values that we have to sent to nod mcu espp8266
int value1;
int value2;
int value3;
int value4;
int value5;
String value6;

#define mq135 A0   //signal pin is connected to A0
#define  mq3  A1   // ==========================A1
#define mq5 A2      // =========================A2
#define buz  4    // BUZZER +VE pin is connected to digital pin 4

// variables that can store sensors data
int mq135_value;
int mq3_value;
int mq5_value;


void setup()
{
  pinMode (  mq135, INPUT ) ;
  pinMode (  mq3, INPUT ) ;
  pinMode (  mq5, INPUT ) ;
  
  pinMode(buz, OUTPUT);
  digitalWrite(buz, LOW);
  
  Serial.begin(115200);
  
  // make the serial link between arduino nano and nod mcu

  linkSerial.begin(4800);

 
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("QUAID-E-AWAM");
  lcd.setCursor(0, 1);
  lcd.print("         UNIVERSITY");
  lcd.setCursor(0, 3);
  lcd.print("   BE  ELECTRONICS  ");
  delay(3000);   // 3 seconds delay 
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("SUNIL       17ES-57");
  lcd.setCursor(0, 1);
  lcd.print("MUMTAZ      17ES-30");
  lcd.setCursor(0, 2);
  lcd.print("KASHIF      17ES-15");
  lcd.setCursor(0, 3);
  lcd.print("NOUMAN      17ES-61");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WIFI:    AIR        ");
  lcd.setCursor(0, 1);
  lcd.print("POLLUTION MONITORING");
  lcd.setCursor(0, 2);
  lcd.print("IP: 192.168.4.1");
  lcd.setCursor(0, 3);
  lcd.print("PASSWORD: 12345678");
  delay(2000);

  lcd.clear();

  dht.begin();
 // Serial.println("DHT OK");

}
void loop()
{
//----------------------------------------------------------------------------------- FOR TEMPERATURE   
int  h = dht.readHumidity();
int   t = dht.readTemperature();
int  f = dht.readTemperature(true);


//---------------------------------------------------------------------------------   FOR SMOKE SENSORS
  int sensorMQ135 = analogRead(mq135);   // READING ANALOG DATA
  mq135_value=map(sensorMQ135, 0, 1023, 0, 100);   // CONVERTE 0-1023 INTO 0-100
  delay(50);
  int sensorMQ3 = analogRead(mq3);
  mq3_value=map(sensorMQ3, 0, 1023, 0, 100);
  delay(50);
  int sensorMQ5 = analogRead(mq5);
  mq5_value=map(sensorMQ5, 0, 1023, 0, 100);
  delay(50);
//----------------------------------------------------------------------------------
  /*
  Serial.print("Humidity       :"); Serial.print(h); Serial.println(" %\t");
  Serial.print("Temperature    :"); Serial.print(t); Serial.println(" *C ");

  Serial.print("MQ-135: "); Serial.print(mq135_value);
  Serial.println(" ");
  Serial.print("MQ-3  : "); Serial.print(mq3_value);
  Serial.println(" ");
  Serial.print("MQ-5  : "); Serial.print(mq5_value);
  Serial.println(" ");

*/
  Serial.println();
// PRINTING TEMPERATURE ON LCD IN CELCIEUS
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("POLLUTION MONITORING");
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.setCursor(2, 1);
  lcd.print(t);
  lcd.setCursor(5, 1);
  lcd.print("C |");
// PRINTING TEMPERATURE ON LCD IN F
  lcd.setCursor(8, 1);
  lcd.print("T:");
  lcd.setCursor(10, 1);
  lcd.print(f);
  lcd.setCursor(13, 1);
  lcd.print("F|");
   
// PRINTING HUMIDITY ON LCD
  lcd.setCursor(15, 1);
  lcd.print("H:");
  lcd.setCursor(17, 1);
  lcd.print(h);
  lcd.setCursor(19, 1);
  lcd.print("%");

  lcd.setCursor(0, 2);
  lcd.print("MQ:135 | MQ:5 | MQ:3");

  lcd.setCursor(7, 3);
  lcd.print("|");
  
  lcd.setCursor(14, 3);
  lcd.print("|");
   
  
  lcd.setCursor(2, 3);
  lcd.print(mq135_value);
  lcd.setCursor(4, 3);
  lcd.print("%");

  lcd.setCursor(10, 3);
  lcd.print(mq5_value);
  lcd.setCursor(12, 3);
  lcd.print("%");

  lcd.setCursor(17, 3);
  lcd.print(mq3_value);
  lcd.setCursor(19, 3);
  lcd.print("%");
  
  delay(1500);   //1.5 SECONDS DELAY
//--------------------------------------------------------------------------------- done with lcd printing
// compare smoke values and set thershold value to get alert 
if (mq135_value > 50  || mq5_value> 50 || mq5_value > 50)
{
//  Serial.println("Smoke alert"); 
  digitalWrite(buz, HIGH);
  delay(300);
  digitalWrite(buz, LOW);
  delay(500);
  digitalWrite(buz, HIGH);
  delay(300);
  digitalWrite(buz, LOW);

  value6 = "SMOKE ALERT"; 
  }
else 
{
//  Serial.print("No smoke alert");
value6 = "NO SMOKE ALERT"; 
}

  // sending all reading to nod mcu to upload on webserver
 // assigning the values to new variables 
 
  value1 = t;   //temperature value
  value2 = h;   // humidity value
  value3 = mq135_value;     // MQ-135 
  value4 = mq3_value;         // MQ-3 value
  value5 = mq5_value;         // MQ-5 value
  
  
  // Print the values on the "debug" serial port
  Serial.print("value1 = ");
  Serial.println(value1);
  Serial.print("value2 = ");
  Serial.println(value2);
  Serial.print("value3 = ");
  Serial.println(value3);
  Serial.print("value4 = ");
  Serial.println(value4);
  Serial.print("value5 = ");
  Serial.println(value5);
  Serial.print("value6 = ");
  Serial.println(value6);


  Serial.println("---");
//

  // Create the JSON document to send towards nod mcu 
  StaticJsonDocument<200> doc;
  doc["value1"] = value1;
  doc["value2"] = value2;
  doc["value3"] = value3;
  doc["value4"] = value4;
  doc["value5"] = value5;
  doc["value6"] = value6;
  // Send the JSON document over the "link" serial port
  serializeJson(doc, linkSerial);

  // Wait
  delay(3000);
  

}
