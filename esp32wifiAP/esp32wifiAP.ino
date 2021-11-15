/*
  Espressif ESP32-S2 WiFi Board Acess Point Test

  This sketch creates a local Access Point from the Espressif ESP32-S2 WiFi board.
  Mobile devices can connect to the local Access Point through Wi-Fi.
  Users can access a web server hosted by the board using a web browser.
  A LED connected to the digital pin 3 can be controlled from the web.
  Data input to the analog pin A1 will be displayed on the web.
  
  This sketch is based on tutorial "How to Set an ESP32 Access Point (AP) for Web Server" 
  provided by (cc) Rui Santos from RandomNerdTutorials.com
  is licensed under CC BY-NC-SA 3.0 <https://creativecommons.org/licenses/by-nc-sa/3.0/>.
  
  Source link:
  https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
  
  Y. Zhu, 2020
  
*/

// Load Wi-Fi library
#include <WiFi.h>

// Create local Acess Point credentials
const char* ssid     = "MahshidLabESP32Test";        // network SSID (name)
const char* password = "ESP32test1234";    // network password (use for WPA, or use as key for WEP)

// Assign output variables to GPIO pins
int led = 3;        // digital pin led
int analogDevice = A1;        // analog pin
int status = WL_IDLE_STATUS;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output3State = "off";

void setup() {
  
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  pinMode(led, OUTPUT);        // set the digital output pin mode
  digitalWrite(led, LOW);        // set the digital output to LOW

  // Print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);
  
  // Create the Acess Point 
  // Remove the password parameter, if the AP Access Point to set to be open
  WiFi.softAP(ssid, password);
  
  // Print IP address
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(IP);
  
  // Wait 10 seconds for connection:
  delay(10000);

  // Start the web server on port 80
  server.begin();
  
}

void loop(){
  
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
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
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 3 - State " + output3State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output3State=="off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            
            // Reading random analog value from an analog pin
            int randomReading = analogRead(analogDevice);
            client.print("Random reading from analog pin: ");
            client.print(randomReading);
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        
        // Check to see if the client request was "GET /3/on" or "GET /3/off"
        // Turn the GPIO on and off accordingly
        if (currentLine.endsWith("GET /3/on")) {
          Serial.println("GPIO 3 on");
          output3State = "on";
          digitalWrite(led, HIGH);
        } else if (currentLine.endsWith("GET /3/off")) {
          Serial.println("GPIO 3 off");
          output3State = "off";
          digitalWrite(led, LOW);
        }
      }
    }
    
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
  }
}
