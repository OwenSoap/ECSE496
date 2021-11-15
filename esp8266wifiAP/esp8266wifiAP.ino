/*
  SparkFun ESP8266 Thing Development Board Acess Point Test

  This sketch creates a local Access Point from the SparkFun ESP8266 Thing Development Board.
  Mobile devices can connect to the local Access Point through Wi-Fi.
  Users can access a web server hosted by the board using a web browser.
  An on-board LED can be controlled from the web.
  Data input to the analog pin A1 and digital pin 12 will be displayed on the web.
  
  This sketch is based on tutorial "ESP8266 Thing Development Board Hookup Guide"
  provided by (cc) jimblom from learn.sparkfun.com
  is licensed under CC BY-SA 4.0 <https://creativecommons.org/licenses/by-sa/4.0/>

  Source link:
  https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/example-sketch-web-server
  
  Y. Zhu, 2020
*/

// Load ESP8266WiFi library
#include <ESP8266WiFi.h>

// Create local Acess Point credentials
const char ssid[] = "MahshidLabESP8266WiFiTest";
const char pass[] = "ESP8266test1234";

// Assign output variables to GPIO pins
const int LED_PIN = 5; // on-board led pin
const int ANALOG_PIN = A0; // analog pin 
const int DIGITAL_PIN = 12; // digital pin for read
// Set web server port number to 80
WiFiServer server(80);

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();       // start the web server on port 80
}

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
  if (req.indexOf("/led/0") != -1)
    val = 1; // Will write LED high
  else if (req.indexOf("/led/1") != -1)
    val = 0; // Will write LED low
  else if (req.indexOf("/read") != -1)
    val = -2; // Will print pin reads
  // Otherwise request will be invalid. We'll say as much in HTML

  // Set GPIO5 according to the request
  if (val >= 0)
    digitalWrite(LED_PIN, val);

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";

  /*Note: Uncomment the line below to refresh automatically
   *      for every 1 second. This is not ideal for large pages 
   *      but for a simple read out, it is useful for monitoring 
   *      your sensors and I/O pins. To adjust the fresh rate, 
   *      adjust the value for content. For 30 seconds, simply 
   *      change the value to 30.*/
  //s += "<meta http-equiv='refresh' content='1'/>\r\n";//auto refresh page

  // If we're setting the LED, print out a message saying we did
  if (val >= 0)
  {
    s += "LED is now ";
    s += (val)?"off":"on";
  }
  else if (val == -2)
  { // If we're reading pins, print out those values:
    s += "Analog Pin = ";
    s += String(analogRead(ANALOG_PIN));
    s += "<br>"; // Go to the next line.
    s += "Digital Pin 12 = ";
    s += String(digitalRead(DIGITAL_PIN));
  }
  else
  {
    s += "Invalid Request.<br> Try /led/1, /led/0, or /read.";
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

void setupWiFi()
{
  // Create the Acess Point 
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
}

void initHardware()
{
  Serial.begin(115200);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);       // set digital pin mode
  pinMode(LED_PIN, OUTPUT);       // set on-board mode
  digitalWrite(LED_PIN, LOW);
}
