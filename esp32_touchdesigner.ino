#include <WiFi.h>
#include <HTTPClient.h>

// This will be the login credentials for our router
const char* ssid = "NYCR24";
const char* password = "clubmate";

// This will be a unique ID for each diffusor
const int sensorID = 1;

// IP address of the host computer + default port of the Web Server DAT in touchdesigner
const char* serverName = "http://192.168.0.73:9980";


// For now, just sends a message every 5 seconds for testing
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Send an HTTP POST request every 5 seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status() == WL_CONNECTED){
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
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}