
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//SSID and Password to your ESP Access Point
const char* ssid = "nodemcu";
const char* password = "123123456";

IPAddress staticIP840_20(10,10,10,10);
IPAddress gateway840_20(192,168,1,1);
IPAddress subnet840_20(255,255,255,0);

ESP8266WebServer server(80); //Server on port 80

//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot() {
  respondToClient();
}

void ledOn() {
  digitalWrite(5,HIGH);
  respondToClient();
}

void ledOff() {
  digitalWrite(5,LOW);
  respondToClient();
}

void toggle() {
  int status = digitalRead(5);
  if (status == HIGH){
    digitalWrite(5,LOW);
  } else {
    digitalWrite(5,HIGH);
  }
  respondToClient();
}

void respondToClient() { 
  int status = digitalRead(5);
  String msg = "off";
  if (status == 1){
    msg = "on";
  }
  String part1 = "<!DOCTYPE HTML><html><head/><p style='font-size: 200%'><body>Status : ";
  String part2 = " </p><a href='http://10.10.10.10/on'><button style='font-size: 800%'>ON</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='http://10.10.10.10/off'><button style='font-size: 800%'>OFF</button></a></body></html>";
  String fullMsg = part1 + msg + part2;
  server.send(200, "text/html", fullMsg);
}

//===============================================================
//                  SETUP
//===============================================================
void setup(void){
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  Serial.println("");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAPConfig(staticIP840_20, gateway840_20, subnet840_20);
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
  

  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpot IP:");
  Serial.println(myIP);
 
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/on", ledOn);
  server.on("/off", ledOff);
  server.on("/toggle", toggle);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//===============================================================
//                     LOOP
//===============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
