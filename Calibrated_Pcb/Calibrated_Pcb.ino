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
const String deviceName = "dryer";

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
  pinMode(relayOverride, INPUT);
}

String fullMsg = "";
long nrfSendDelay = 57000;
unsigned long timeNow;
unsigned long start = millis();

// for current sensor
unsigned long samplingTime = 5600; 
unsigned long ctStart = millis(); 
double ampArray[10]; 
int ctCount = 0; 
const int analogIn = A5;
int mVperAmp = 100; // use 100 for 20A Module and 66 for 30A Module
int RawValue = 0;
int ACSoffset = 2500;
double Voltage = 0;
double Amps = 0;
double mxm;
void loop()          {
  // wdt_reset();                    //reset watch dog timer to prevenet timeout

  ADE7758 meter;

  meter.setSPI();
  unsigned long currentMillis = millis(); // grab current time

  // check if "interval" time has passed (1000 milliseconds)
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {

    //    VOLTAGE REGISTER
    long vv = (meter.read24(AVRMS));
    Serial.print("Voltage Register = ");
    Serial.println(vv);
    double y2 = (0.0002 * vv) - 1.7466;
    if (vv < 18000) {
      y2 = 0;
    }
    Serial.print("Voltage Reading==");
    Serial.println(y2);

    //    CURRENT REGISTER
//    long aa = (meter.read24(AIRMS));
//    Serial.print("Current Register = ");
//    Serial.println(aa);
//    double y1 = (0.00001 * aa) + 0.0427;
//    if (aa < 1000) {
//      y1 = 0;
//    }
//    Serial.print("Current Reading= ");
//    Serial.println(y1);

    //    TRANSMIT POWER
    timeNow = millis();
    if ((timeNow - start) > nrfSendDelay) {
      radio.stopListening();
      mxm = ampArray[0];
      for (int i = 0; i < 10; i++) {
       if (ampArray[i] > mxm) {
         mxm = ampArray[i];
       }
      }
      ctCount = 0;
      double mxmrms = (mxm)*(0.707);
      Serial.print("Max current Sensor Reading= ");
      Serial.println(mxmrms);

      //    Power Calculation
      double pp = (y2) * (mxmrms);
      Serial.print("Power Reading = ");
      Serial.println(pp);
      
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

  //current sensor logic
  if (((millis() - ctStart) > samplingTime) && ctCount < 10) {
    RawValue = analogRead(analogIn);
    Voltage = (RawValue / 1024.0) * 5000; // Gets you mV
    Amps = ((Voltage - ACSoffset) / mVperAmp);
    Serial.print("Current Sensor Reading= ");
    Serial.println(Amps);
    ampArray[count] = Amps;
    ctCount = ctCount + 1;
    ctStart = millis();
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
        String sts = fullMsg.substring(6, 7);
        Serial.print("dryer=");
        Serial.println(sts);
        if (sts == "1") {
          digitalWrite(relayPin, LOW);
        } else {
          digitalWrite(relayPin, HIGH);
        }
      } else if (deviceName == "fan") {
        String sts = fullMsg.substring(12, 13);
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



