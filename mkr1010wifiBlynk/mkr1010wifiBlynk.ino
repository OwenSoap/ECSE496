/*
  Arduino MKR WiFi 1010 Board Test

  This sketch connects the Arduino MKR WiFi 1010 board to the Internet through Wi-Fi.
  Mobile devices connected to the Internet can perform various operations and read 
  virtual LCD data trough an application named Blynk.
  
  This sketch is based on tutorial "Control Your IoT Cloud Kit via Blynk"
  provided by (c) officine-innesto & (c) Arduino_Genuino from Arduino Project Hub
  is licensed under CERN-OHL v1.2 <https://ohwr.org/project/cernohl/wikis/Documents/CERN-OHL-version-1.2>.

  Source link:
  https://create.arduino.cc/projecthub/officine-innesto/control-your-iot-cloud-kit-via-blynk-ec6a16
  
  Y. Zhu, 2020
*/

#define BLYNK_PRINT Serial

// Load libraries
#include <SPI.h>
#include <WiFiNINA.h>
#include <Ethernet.h>
#include <BlynkSimpleWiFiNINA.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "(Token received)";

// WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "(Any 2.4G Hz WiFi SSID)";
char pass[] = "(Password)";

WidgetLCD lcd(V1);

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  
  lcd.clear(); // Use it to clear the LCD Widget
  lcd.print(3, 0, "WiFi test"); // use: (position X: 0-15, position Y: 0-1, "(Messages)")
  lcd.print(2, 1, "Mahshid Lab");
}

void loop()
{
  Blynk.run();
}
