#include <WiFi.h>
#include <HTTPClient.h>

#define sckPin 8
#define dataPin 10
#define THRESHOLD 10000000

// This will be the login credentials for our router
const char* ssid = "scents_of_us";
const char* password = "smellovision";

// This will be a unique ID for each diffusor
const int sensorID = 1;

// IP address of the host computer + default port of the Web Server DAT in touchdesigner
const char* serverName = "http://192.168.8.2:9980";

// tracks the previous state of the sensor for looping
bool previouslyActive = false;

void sendTriggerEvent() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST(String(sensorID));


    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(dataPin, INPUT);  // Connect HX710 OUT to Arduino pin 10
  pinMode(sckPin, OUTPUT);  // Connect HX710 SCK to Arduino pin 8


  // Wifi Setup
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {


  // wait for the current reading to finish
  while (digitalRead(dataPin)) {}

  // read 24 bits
  long result = 0;
  for (int i = 0; i < 24; i++) {
    digitalWrite(sckPin, HIGH);
    digitalWrite(sckPin, LOW);
    result = result << 1;
    if (digitalRead(dataPin)) {
      result++;
    }
  }

  // get the 2s compliment
  result = result ^ 0x800000;

  // pulse the clock line 3 times to start the next pressure reading
  for (char i = 0; i < 3; i++) {
    digitalWrite(sckPin, HIGH);
    digitalWrite(sckPin, LOW);
  }

  // display pressure
  if (int(result) > THRESHOLD) {
    if (previouslyActive == false) {
      previouslyActive = true;
      Serial.println("Triggered");
      sendTriggerEvent();
    }
  } else {
    previouslyActive = false;
  }
}