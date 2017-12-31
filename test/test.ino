#include <SPI.h>
void setup() {
  Serial.begin(9600);
  String fullMsg = "asd,sdf,dfg,fgh";
  char * s;
  fullMsg.toCharArray(s, (fullMsg.length()+1));
  char * ss;
  ss = strtok_r(s, ",", &s);
  Serial.println(ss);
  ss = strtok_r(s, ",", &s);
  Serial.println(ss);
  ss = strtok_r(s, ",", &s);
  Serial.println(ss);
  ss = strtok_r(s, ",", &s);
  Serial.println(ss);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
