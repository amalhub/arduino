#include <ESP8266WiFi.h>

String  ClientRequest;
IPAddress staticIP840_20(192,168,43,20);
IPAddress gateway840_20(192,168,1,1);
IPAddress subnet840_20(255,255,255,0);

WiFiServer server(80);

void setup()
{
  ClientRequest = "";

  Serial.begin(9600);

  pinMode(16, OUTPUT);
    WiFi.disconnect();
  Serial.println("START");
   WiFi.begin("DAAG","6789054321");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print(".");

  }
  Serial.println("");
  Serial.println("Connected");
  WiFi.config(staticIP840_20, gateway840_20, subnet840_20);
  Serial.println("Your IP is:");
  Serial.println((WiFi.localIP()));
  server.begin();

}


void loop()
{

    WiFiClient client = server.available();
    if (!client) { return; }
    while(!client.available()){  delay(1); }
    ClientRequest = (client.readStringUntil('\r'));
    ClientRequest.remove(0, 5);
    ClientRequest.remove(ClientRequest.length()-9,9);
    if (ClientRequest.equals("on")) {
      digitalWrite(16,LOW);

    }
    if (ClientRequest.equals("off")) {
      digitalWrite(16,HIGH);

    }
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("</head>");
    client.println("<body>");
      client.println("<a href=");
      client.println("http://192.168.43.20/on");
      client.println("><button>");
      client.println("On");
      client.println("</button></a>");
      client.println("<a href=");
      client.println("http://192.168.43.20/off");
      client.println("><button>");
      client.println("Off");
      client.println("</button></a>");
    client.println("</body>");
    client.println("</html>");
    client.stop();
     delay(1);

}
