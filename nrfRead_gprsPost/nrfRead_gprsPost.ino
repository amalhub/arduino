#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

//Tx=9, Rx=10 (GPRS)
SoftwareSerial mySerial(9, 10);
long statusReadWait = 23000;

//CE=7, CSN=8 (NRF)
RF24 radio(7,8);
byte addresses[][6] = {"1Node","2Node"};
const int roleName = 2;

void setup() {
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  radio.begin();
  if (roleName == 1) {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  } else {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
  }
  radio.startListening();
  delay(500);
  initGPRS();
}

unsigned long timeNow;
unsigned long start = millis();
String fullMsg = "";
void loop() {
  timeNow = millis();
  if((timeNow - start) > statusReadWait){
    getStatus();
    start = millis();
  }
  radioListen();
}

void radioListen() {
  if(radio.available()) {
    int value;
    radio.read(&value, 1);
    char theChar = value;
    if(theChar != '`') {
      fullMsg.concat(theChar);
    } else {
      Serial.print("NRF msg = ");
      Serial.println(fullMsg);
      char charArr[fullMsg.length()+1];
      char * pointer = charArr;
      fullMsg.toCharArray(charArr, (fullMsg.length()+1));
      char * address = strtok_r(pointer, ",", &pointer);
      char * device = strtok_r(pointer, ",", &pointer);
      char * registry = strtok_r(pointer, ",", &pointer);
      char * valueStr = strtok_r(pointer, ",", &pointer);
      String data = String ("{\"address\":\"") + String(address) + ("\",\"applianceName\":\"") 
        + String(device) + ("\",\"registerName\":\"") + String(registry) + ("\",\"value\":") 
        + String(valueStr) + String("}");
      Serial.println(data);
      postData(data);
      fullMsg = "";
    }
  }
}

String serverUrl = "http://drm.netne.net";
void postData(String data){
  String httpData = String("AT+HTTPDATA=") + String(data.length()) + String(",5000");
  String postCommands[] = {
    "AT+HTTPPARA=\"CID\",1",
    String("AT+HTTPPARA=\"URL\",\"") + serverUrl + String("\""),
    "AT+HTTPPARA=\"CONTENT\",\"application/json\"",
    httpData,
    data,
    "AT+HTTPACTION=1",
    "AT+HTTPREAD"
  };
  for (int i = 0; i < (sizeof(postCommands)/sizeof(String)); i++) {
    sendCommand(postCommands[i]);
    Serial.print(readResponse());
  }
}

String statusUrl = "http://drm.netne.net/status.php";
void getStatus(){
  String postCommands[] = {
    "AT+HTTPPARA=\"CID\",1",
    String("AT+HTTPPARA=\"URL\",\"") + statusUrl + String("\""),
    "AT+HTTPACTION=0",
    "AT+HTTPREAD"
  };
  for (int i = 0; i < (sizeof(postCommands)/sizeof(String)); i++) {
    sendCommand(postCommands[i]);
    String result = readResponse();
    int foundIndex = result.indexOf("dry");
    if(foundIndex >= 0) {
      String relayMsg = result.substring(foundIndex, (foundIndex+13));
      radio.stopListening();
      //Sending data to NRF
      int msgSize = relayMsg.length();
      Serial.print("Sending msg: ");
      Serial.println(relayMsg);
      for (int i = 0; i < msgSize; i++) {
        int charToSend;
        charToSend = relayMsg.charAt(i);
        if (!radio.write(&charToSend, 1)) {
          Serial.println("failed to send.");
        }
        delay(100);
      }
       //send the 'terminate string' value...
      int charToSend;
      charToSend = '`'; 
      if (!radio.write(&charToSend, 1)) {
        Serial.println("failed to send.");
      }
      delay(100);
      radio.startListening();
    }
    Serial.print(result);
  }
}


void initGPRS(){
  String initCommands[] = {
    "AT+SAPBR=3,1,\"Contype\",\"GPRS\"",
    "AT+SAPBR=3,1,\"APN\",\"mobitel3g\"",
    "AT+SAPBR=1,1",
    "AT+SAPBR=2,1",
    "AT+HTTPINIT"
  };
  for (int i = 0; i < (sizeof(initCommands)/sizeof(String)); i++) {
    sendCommand(initCommands[i]);
    Serial.print(readResponse());
  }
}

void sendCommand(String command){
  Serial.println("Sending: " + command);
  mySerial.println(command);
  //myDelay(millis(), 2000);
  delay(2000);
}

String readResponse(){
  String response = "";
  while(mySerial.available()>0){
    String res = mySerial.readString();
    response += res;
  }
  delay(100);
  return response;
}

void myDelay(long startTime, long wait) {
  timeNow = millis();
  while((timeNow - startTime) <= wait){
    timeNow = millis();
    radioListen();
  } 
  return;
}

