#include <ADE7758.h>

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
const String deviceName = "fan";

unsigned long TimeStampSinceLastReboot;
unsigned long previousWdtMillis = 0;
unsigned long wdtInterval       = 0;


void setup()          {
  //int relay = 4;
  //  pinMode(relay,OUTPUT);
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
    Serial.print("Voltage Register==");
    Serial.println(vv);

    //    CURRENT REGISTER
    long aa = (meter.read24(AIRMS));
    Serial.print("Current_reading= ");
    Serial.println(aa);

    //    CURRENT REGISTER
    long pp = (vv) * (aa) * (0.975);
    Serial.print("Power Register = ");
    Serial.println(pp);

    //    TRANSMIT POWER
    timeNow = millis();
    if((timeNow - start) > nrfSendDelay){
      radio.stopListening();
      String msg = String("nrf,") + deviceName + String(",AWATTHR,") + String(pp);
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
        Serial.println(sts);
        if (sts == "1") {
          digitalWrite(relayPin, HIGH);
        } else {
          digitalWrite(relayPin, LOW);
        }
      } else if (deviceName == "fan") {
        String sts = fullMsg.substring(12,13);
        Serial.println(sts);
        if (sts == "1") {
          digitalWrite(relayPin, HIGH);
        } else {
          digitalWrite(relayPin, LOW);
        }
      }
      fullMsg = "";
    }
    delay(100);
  }
}



