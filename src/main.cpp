#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

#define LED 2

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);    // new web server that listens for HTTP requests on port 80

void handleRoot();
void handleNotFound();
void handleLED();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("your_wifi_network", "your_wifi_password");

  Serial.println("Connecting ...");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(250);
    Serial.print('.');
  }

  Serial.println('\n');
  Serial.print("Connected to:\t");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address:\t");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up the mDNS responder!!!!");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.on("/LED", HTTP_POST, handleLED);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}

void handleLED() {
  digitalWrite(LED, !digitalRead(LED));
  server.sendHeader("Location", "/");
  server.send(303);
}