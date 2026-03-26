#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// --- Wi-Fi Settings ---
const char* ssid = "YOUR_WIFI_SSID";          // Put your Wi-Fi name here
const char* password = "YOUR_WIFI_PASSWORD";  // Put your Wi-Fi password here

// Create a web server listening on standard HTTP port 80
WebServer server(80);

// --- Hardware Pins & Timing ---
const int sampleTimeinMs = 50; // Lowered delay to make web server more responsive
const int baudRate = 9600;

const int wlanLedPin = 19;

const int blueButtonPin = 17;   // Pump ON
const int whiteButtonPin = 16;  // Pump OFF
const int greenButtonPin = 4;   // Valve ON
const int redButtonPin = 13;    // Valve OFF

const int buttonPins[] = {
    blueButtonPin,
    whiteButtonPin,
    greenButtonPin,
    redButtonPin,
};
const int numButtonPins = 4;

const int valveTriggerPin = 26;
const int pumpTriggerPin = 27;

// --- Web Server Command Handlers ---
void handlePumpOn() {
  digitalWrite(pumpTriggerPin, HIGH);
  Serial.println("Remote Command: Pump ON");
  server.send(200, "application/json", "{\"status\":\"success\", \"device\":\"pump\", \"state\":\"on\"}");
}

void handlePumpOff() {
  digitalWrite(pumpTriggerPin, LOW);
  Serial.println("Remote Command: Pump OFF");
  server.send(200, "application/json", "{\"status\":\"success\", \"device\":\"pump\", \"state\":\"off\"}");
}

void handleValveOn() {
  digitalWrite(valveTriggerPin, HIGH);
  Serial.println("Remote Command: Valve ON");
  server.send(200, "application/json", "{\"status\":\"success\", \"device\":\"valve\", \"state\":\"on\"}");
}

void handleValveOff() {
  digitalWrite(valveTriggerPin, LOW);
  Serial.println("Remote Command: Valve OFF");
  server.send(200, "application/json", "{\"status\":\"success\", \"device\":\"valve\", \"state\":\"off\"}");
}

void setup() {
  Serial.begin(baudRate);

  // Initialize Pins
  for (int i = 0; i < numButtonPins; i++) {
      pinMode(buttonPins[i], INPUT_PULLDOWN);
  }
  pinMode(wlanLedPin, OUTPUT);
  pinMode(valveTriggerPin, OUTPUT);
  pinMode(pumpTriggerPin, OUTPUT);

  // 1. Connect to Wi-Fi
  WiFi.begin(ssid, password);
  if (!MDNS.begin("pump")) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started: http://pump.local");
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // 2. Wi-Fi Connected
  Serial.println("\nConnected to WiFi!");
  Serial.print("ESP32 Pump IP Address: ");
  Serial.println(WiFi.localIP()); // YOU WILL NEED THIS IP ADDRESS
  digitalWrite(wlanLedPin, HIGH); 

  // 3. Define the REST API endpoints
  server.on("/api/pump/on", HTTP_GET, handlePumpOn);
  server.on("/api/pump/off", HTTP_GET, handlePumpOff);
  server.on("/api/valve/on", HTTP_GET, handleValveOn);
  server.on("/api/valve/off", HTTP_GET, handleValveOff);

  // 4. Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // 1. Listen for incoming HTTP commands from the Raspberry Pi
  server.handleClient();

  // 2. Listen for physical button presses (Manual Override)
  if (digitalRead(blueButtonPin) == HIGH) {
      digitalWrite(pumpTriggerPin, HIGH);
      Serial.println("Manual Override: Pump ON");
  }

  if (digitalRead(whiteButtonPin) == HIGH) {
      digitalWrite(pumpTriggerPin, LOW);
      Serial.println("Manual Override: Pump OFF");
  }

  if (digitalRead(greenButtonPin) == HIGH) {
      digitalWrite(valveTriggerPin, HIGH);
      Serial.println("Manual Override: Valve ON");
  }

  if (digitalRead(redButtonPin) == HIGH) {
      digitalWrite(valveTriggerPin, LOW);
      Serial.println("Manual Override: Valve OFF");
  }

  // Safety check: if Wi-Fi drops, turn off the LED
  if (WiFi.status() != WL_CONNECTED) {
     digitalWrite(wlanLedPin, LOW);
  } else {
     digitalWrite(wlanLedPin, HIGH);
  }

  delay(sampleTimeinMs);
}