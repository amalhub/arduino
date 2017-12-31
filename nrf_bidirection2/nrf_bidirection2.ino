#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

//CE=7, CSN=8 (NRF)
RF24 radio(7,8);
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

const int roleName = 2;

void setup() {
  Serial.begin(9600);
  radio.begin();
  if (roleName == 1) {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
  } else {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1, pipes[0]);
  }
  radio.printDetails();  // Dump the configuration of the rf unit for debugging

  radio.powerUp();
  radio.startListening();
  delay(100);
  while (! Serial); // Wait until Serial is ready
}

void loop() {
  if (Serial.available()) {
    long mgs = (long) Serial.read();
    radio.stopListening();
    delay(100);
    if (!radio.write(&mgs, sizeof(long))) {
      Serial.println("failed to send.");
    }
    radio.startListening();
    delay(100);
  }
  if(radio.available()) {
    Serial.print("NRF msg = ");
    long value;
    radio.read(&value, sizeof(long));
    Serial.println(value);
  }
}
