#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> // Include the WebServer library
#include <Servo.h>

#define ServoPin 14 // D5 --> GPIO14

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80); // new web server that listens for HTTP requests on port 80

Servo myservo;

void handleNotFound();
void handleServo();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');

  // Connect to WiFi
  /*
   * Change the below to your own WiFi Details
  */
  wifiMulti.addAP("Your_WiFi_Network", "your_wifi_password"); 
  Serial.println("Connecting ...");
  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(250);
    Serial.print('.');
  }

  // If connection successful, print details
  Serial.println('\n');
  Serial.print("Connected to:\t");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address:\t");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266"))
  {
    Serial.println("mDNS responder started");
  }
  else
  {
    Serial.println("Error setting up the mDNS responder!!!!");
  }

  // Web Server Setup & Initiliaziation
  server.onNotFound(handleNotFound);
  server.on("/motor/spin", handleServo);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not Found");
}

void handleServo()
{
  myservo.attach(ServoPin);
  // URI Args
  String servoSpeed = server.arg("servoSpeed");
  String spinTime = server.arg("spinTime");

  int speed = servoSpeed.toInt(); // Value for the speed of the servo
  int time = spinTime.toInt();    // Value for the delay (how long we will let it spin before it stops) in milliseconds

  Serial.println("spintime = " + spinTime);

  myservo.write(speed);
  delay(time);
  myservo.detach();

  server.send(200, "text/plain");
}