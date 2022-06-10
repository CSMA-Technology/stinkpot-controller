#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <LittleFS.h>   // Include the LittleFS library - SPIFFS is deprecated

#define LED 2

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);    // new web server that listens for HTTP requests on port 80

// FLASH_MAP_SETUP_CONFIG(FLASH_MAP_MAX_FS);

void handleRoot();
void handleNotFound();
void handleLED();
bool handleFileRead(String path);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("_3307_", "MindOv3rMatter");

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

  LittleFS.begin(); // Start the File System

  // server.on("/", handleRoot);
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      handleNotFound();
    }
  });
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
  // server.sendHeader("Location", "/");
  // server.send(303);
}

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) {
    path += "index.html";
  }
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    size_t sent = server.streamFile(file, "text/html");
    file.close();
    return true;
  }
  Serial.println("\tFile not found");
  return false;
}