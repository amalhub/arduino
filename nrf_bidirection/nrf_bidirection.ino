#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

//CE=7, CSN=8 (NRF)
RF24 radio(7,8);
byte addresses[][6] = {"1Node","2Node"};

const int roleName = 2;

void setup() {
  Serial.begin(9600);
  radio.begin();
  if (roleName == 1) {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  } else {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
  }
  radio.startListening();
  delay(100);
}

String msg = ""; 
String fullMsg = "";
void loop() {
  if (Serial.available()) {
    radio.stopListening();
    msg = Serial.readString();
    int msgSize = msg.length();
    Serial.print("Sending msg: ");
    Serial.println(msg);
    for (int i = 0; i < msgSize; i++) {
      int charToSend;
      charToSend = msg.charAt(i);
      if (!radio.write(&charToSend, 1)) {
        Serial.println("failed to send.");
      }
    }
     //send the 'terminate string' value...
    int charToSend;
    charToSend = '`'; 
    if (!radio.write(&charToSend, 1)) {
      Serial.println("failed to send.");
    }
    radio.startListening();
  }
  if(radio.available()) {
    int value;
    radio.read(&value, 1);
    char theChar = value;
    if(theChar != '`') {
      fullMsg.concat(theChar);
    } else {
      Serial.print("NRF msg = ");
      Serial.println(fullMsg);
      fullMsg = "";
    }
  }
}
