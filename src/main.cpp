#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <LittleFS.h>   // Include the LittleFS library - SPIFFS is deprecated
#include <Servo.h>

#define LED 2 // LED on GPIO2
#define ServoPin 14 // D5 --> GPIO14

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);    // new web server that listens for HTTP requests on port 80

Servo myservo;

void handleNotFound();
void handleLED();
bool handleFileRead(String path);
void handleServo();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  delay(10);
  Serial.println('\n');

  // Attach Servo
  myservo.attach(ServoPin);

  // Connect to WiFi
  wifiMulti.addAP("your_wifi_network", "your_wifi_password");
  Serial.println("Connecting ...");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(250);
    Serial.print('.');
  }

  // If connection successful, print details
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

  // Start the File System
  LittleFS.begin();

  // Web Server Setup & Initiliaziation
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      handleNotFound();
    }
  });
  server.on("/LED", HTTP_POST, handleLED);
  server.on("/motor/setPOS", handleServo);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}

void handleLED() {
  digitalWrite(LED, !digitalRead(LED));
  server.sendHeader("Location", "/");
  server.send(303);
}

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) {
    path += "index.html";
  }
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    server.streamFile(file, "text/html");
    file.close();
    return true;
  }
  Serial.println("\tFile not found");
  return false;
}

void handleServo() {
  String POS = server.arg("servoPOS");
  int pos = POS.toInt();
  myservo.write(pos); // Tell servo to go to the given position
  Serial.print("Servo angle: ");
  Serial.println(pos);
  digitalWrite(LED, !(digitalRead(LED)));
  server.send(200, "text/plain");
}