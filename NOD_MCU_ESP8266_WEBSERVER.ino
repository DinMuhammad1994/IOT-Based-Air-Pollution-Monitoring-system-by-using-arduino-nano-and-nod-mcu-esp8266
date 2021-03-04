
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

IPAddress    apIP(192, 168, 4, 1);  // Defining a static IP address: local & gateway
                                    // Default IP in AP mode is 192.168.4.1

/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "AIR POLLUTION MONITORING";
const char *password = "12345678";

// Define a web server at port 80 for HTTP
ESP8266WebServer server(80);

#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(D6, D5); // RX, TX

String value1;
String value2;
String value3;
String value4;
String value5;
String value6;

String page = "";
double data;
void setup(void) {

  Serial.begin(115200);

  // Initialize the "link" serial port
  // Use the lowest possible data rate to reduce error ratio
  linkSerial.begin(4800);

  Serial.println();
  Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", []() {
    page = "<h1 style=\"text-align: center;\"><span style=\"text-decoration: underline;\"><span style=\"color: #ff9900; text-decoration: underline;\">IOT BASED AIR POLLUTION MONITORING SYSTEM</span></span></h1>"
           "<h4><span style='color: #808080;'>TEMPERATURE (C): " + String(value1) +
           "<h4><span style='color: #808080;'>HUMIDITY    (%) :  "  + String(value2) +
           "<h4><span style='color: #808080;'>SMOKE SENSOR MQ-135 (%) : " + String(value3) +
           "<h4><span style='color: #808080;'>SMOKE SENSOR MQ-3   (%) : " + String(value4) +
           "<h4><span style='color: #808080;'>SMOKE SENSOR MQ-5   (%) : " + String(value5) +
           "<h4><span style='color: #808080;'>SMOKE ALERT : " + (value6) +
           "<meta http-equiv=\"refresh\" content=\"5\" >";
        
         
    server.send(200, "text/html", page);
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop(void) {


  if (linkSerial.available())
  {
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<300> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, linkSerial);

    if (err == DeserializationError::Ok)
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      Serial.print("value1 = ");
      Serial.println(doc["value1"].as<int>());
      Serial.print("value2 = ");
      Serial.println(doc["value2"].as<int>());
      Serial.print("value3 = ");
      Serial.println(doc["value3"].as<int>());
      Serial.print("value4 = ");
      Serial.println(doc["value4"].as<int>());
      Serial.print("value5 = ");
      Serial.println(doc["value5"].as<int>());
     Serial.print("value6 = ");
     Serial.println(doc["value6"].as<String>());


      value1 = (doc["value1"].as<int>());
      value2 = (doc["value2"].as<int>());
      value3 = (doc["value3"].as<int>());
      value4 = (doc["value4"].as<int>());
      value5 = (doc["value5"].as<int>());
      value6 = (doc["value6"].as<String>());
    }
    else
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      // Flush all bytes in the "link" serial port buffer
      while (linkSerial.available() > 0)
        linkSerial.read();
    }
  }


  server.handleClient();
}
