#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>
#include "SPI.h"
#include <avr/pgmspace.h>
#include <avr/wdt.h>                    //watchdog timer
#include "ADE7758.h"


unsigned long interval = 10000; // the time we need to wait
unsigned long previousMillis = 0; // millis() returns an unsigned long.

//CE=7, CSN=8 (NRF)
RF24 radio(7, 8);
byte addresses[][6] = {"1Node", "2Node"};
const int roleName = 1;

const unsigned int relayPin = 4;
const unsigned int relayOverride = 2;
const String deviceName = "fan";

unsigned long TimeStampSinceLastReboot;
unsigned long previousWdtMillis = 0;
unsigned long wdtInterval       = 0;


void setup()          {
  ADE7758 meter;
  //          wdt_disable();
  Serial.begin(9600);
  //TimeStampSinceLastReboot = millis();
  Serial.println("BEGIN=============");
  meter.setSPI();

  //  meter.setAVRMSOS(0x049);
  meter.closeSPI();
  //
  radio.begin();
  if (roleName == 1) {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  } else {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
  }
  radio.startListening();
  pinMode(relayPin, OUTPUT);
  pinMode(relayOverride, INPUT);
}

String fullMsg = "";
long nrfSendDelay = 57000;
unsigned long timeNow;
unsigned long start = millis();
void loop()          {
  // wdt_reset();                    //reset watch dog timer to prevenet timeout

  ADE7758 meter;

  meter.setSPI();
  unsigned long currentMillis = millis(); // grab current time

  // check if "interval" time has passed (1000 milliseconds)
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {

    //    VOLTAGE REGISTER
    long vv = (meter.read24(AVRMS));
    double y2 = (0.0002*vv)-1.7466;
    if (y2 < 0) {
      y2 = 0;
    }
    Serial.print("Voltage Reading==");
    Serial.println(y2);

    //    CURRENT REGISTER
    long aa = (meter.read24(AIRMS));
    double y1 = (0.00001*aa)+0.0427;
    if ( aa < 800 ) {
      y1 = 0;
    }
    Serial.print("Current Reading= ");
    Serial.println(y1);
    
    //    CURRENT REGISTER
    double pp = (y2) * (y1);
    Serial.print("Power Reading = ");
    Serial.println(pp);

    //    TRANSMIT POWER
    timeNow = millis();
    if((timeNow - start) > nrfSendDelay){
      radio.stopListening();
      String msg = String("0000,") + deviceName + String(",AWATTHR,") + String(pp);
      if (digitalRead(relayOverride) == HIGH) {
        msg = String("1111,") + deviceName + String(",AWATTHR,") + String(pp);
      }
      int msgSize = msg.length();
      Serial.print("Sending msg: ");
      Serial.println(msg);
      for (int i = 0; i < msgSize; i++) {
        int charToSend;
        charToSend = msg.charAt(i);
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
      start = millis();

    }
    delay(100);

    //meter.closeSPI();
    Serial.println("\t");
    previousMillis = millis();
  }
  
  //    RECEIVE RELAY SIGNAL
  if (radio.available()) {
    int value;
    radio.read(&value, 1);
    char theChar = value;
    if (theChar != '`') {
      fullMsg.concat(theChar);
    } else {
      Serial.print("NRF msg = ");
      Serial.println(fullMsg);
      if (deviceName == "dryer") {
        String sts = fullMsg.substring(6,7);
        Serial.print("dryer=");
        Serial.println(sts);
        if (sts == "1") {
          digitalWrite(relayPin, LOW);
        } else {
          digitalWrite(relayPin, HIGH);
        }
      } else if (deviceName == "fan") {
        String sts = fullMsg.substring(12,13);
        Serial.print("fan=");
        Serial.println(sts);
        if (sts == "1") {
          digitalWrite(relayPin, LOW);
        } else {
          digitalWrite(relayPin, HIGH);
        }
      }
      fullMsg = "";
    }
    delay(100);
  }
}



