#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include "Nextion.h"

//CE=7, CSN=8 (NRF)
RF24 radio(7,8);
byte addresses[][6] = {"1Node","2Node"};
const int roleName = 2;

//Display
NexText unit1 = NexText(1, 8, "t6");
NexText unit2 = NexText(1, 9, "t7");
NexText total = NexText(1, 10, "t8");

void setup() {
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
  nexInit();
  delay(1000); 
  unit1.setText("0.0");
  unit2.setText("0.0");
  total.setText("0.0"); 
}

String fanDisplay="0.0";
String dryerDisplay="0.0";
String fullMsg = "";
void loop() {
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
      String de = device;
      if (de == "fan") {
        fanDisplay = valueStr;
      } else {
        dryerDisplay = valueStr;
      }
      String totalStr = String(fanDisplay.toInt() + dryerDisplay.toInt());
      char ff[fanDisplay.length()+1];
      char dd[dryerDisplay.length()+1];
      char tt[totalStr.length()+1];
      fanDisplay.toCharArray(ff, fanDisplay.length()+1); 
      dryerDisplay.toCharArray(dd, dryerDisplay.length()+1);
      totalStr.toCharArray(tt, totalStr.length()+1);
      total.setText(tt);
      delay(100);
      unit1.setText(ff);
      delay(100);
      unit2.setText(dd);
      delay(100);
      total.setText(tt);
      delay(100);
      Serial.println();
      fullMsg = "";
    }
  }
}
