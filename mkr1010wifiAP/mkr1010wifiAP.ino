/*
  Arduino MKR WiFi 1010 Board Acess Point Test

  This sketch creates a local Access Point from the Arduino MKR WiFi 1010 board.
  Mobile devices can connect to the local Access Point through Wi-Fi.
  Users can access a web server hosted by the board using a web browser.
  A LED connected to the digital pin 3 can be controlled from the web.
  Data input to the analog pin A1 will be displayed on the web.
  
  This sketch is based on tutorial "Web Server using Access Point (AP) mode with MKR WiFi 1010"
  provided by (cc) Karl Söderby from Arduino
  is licensed under CC BY-NC-SA 3.0 <https://creativecommons.org/licenses/by-nc-sa/3.0/>.
  
  Source link:
  https://www.arduino.cc/en/Guide/MKRWiFi1010/web-server-ap-mode
  
  Y. Zhu, 2020
*/

// Load WiFiNINA library
#include <WiFiNINA.h>

// Create local Acess Point credentials
char ssid[] = "MahshidLabMKR1010WiFiTest";        // network SSID (name)
char pass[] = "MKR1010test1234";    // network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // network key Index number (needed only for WEP)

// Assign output variables to GPIO pins
int analogDevice = A1;        // analog pin    
// int led =  LED_BUILTIN;        // on-borad led
int led2 = 3;        // digital pin led
int status = WL_IDLE_STATUS;

// Set web server port number to 80
WiFiServer server(80);

void setup() {
   
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

  // pinMode(led, OUTPUT);      // set the led pin mode
  pinMode(led2, OUTPUT);        // set the digital output pin mode
  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // Check for WiFi firmware
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // Print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create the Acess Point 
  // Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // Wait 10 seconds for connection:
  delay(10000);

  // Start the web server on port 80
  server.begin();

  // Print out WiFi status
  printWiFiStatus();
}

void loop() {
  // Compare the previous status to the current status
  if (status != WiFi.status()) {
    // iI has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // The content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");
      
            // Reading random analog value from an analog pin
            int randomReading = analogRead(analogDevice);
            client.print("Random reading from analog pin: ");
            client.print(randomReading);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L"
        // Turn the GPIO on and off accordingly
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(led2, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(led2, LOW);                // GET /L turns the LED off
        }
      }
    }
    // Close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWiFiStatus() {
  // Print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
