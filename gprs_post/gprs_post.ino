#include <SoftwareSerial.h>

//Tx=9, Rx=10
SoftwareSerial mySerial(9, 10);
bool read;
String data = "{\"applianceName\":\"fan\",\"registerName\":\"AVRMS\",\"value\":1234}";
String httpData = String("AT+HTTPDATA=") + String(data.length()) + String(",5000");
String commands[] = {
    "AT+SAPBR=3,1,\"Contype\",\"GPRS\"",
    "AT+SAPBR=3,1,\"APN\",\"mobitel3g\"",
    "AT+SAPBR=1,1",
    "AT+SAPBR=2,1",
    "AT+HTTPINIT",
    "AT+HTTPPARA=\"CID\",1",
    "AT+HTTPPARA=\"URL\",\"http://drm.netne.net/\"",
    "AT+HTTPPARA=\"CONTENT\",\"application/json\"",
    httpData,
    data,
    "AT+HTTPACTION=1",
    "AT+HTTPREAD",
    "AT+HTTPTERM"
  };
int nextCmd = 0;
void setup() {
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  read = false;
  delay(1000);
}

void loop() {
  if(read){
    if (mySerial.available()>0) {
      Serial.write(mySerial.read());
      if(mySerial.available()<=0 && (sizeof(commands)/sizeof(String)) > nextCmd) {
        read = false;
      }
    }     
  }else{
    Serial.print(nextCmd);
    Serial.println("Sending: " + commands[nextCmd]);
    mySerial.println(commands[nextCmd]);
    delay(2000);
    nextCmd++;
    read = true;
  }
}
