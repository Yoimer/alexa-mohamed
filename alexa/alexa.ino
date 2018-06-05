/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "A1E-Z4kgHL1BHoC5rgQ5sH0Wcey1H8JRf1" // Your Ubidots TOKEN
#define WIFINAME "Casa" //Your SSID
#define WIFIPASS "remioy2006202" // Your Wifi Pass
// global values for humidity and temperature
float h = -1.0;
float t = -1.0;

Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  if ((char)payload[0]=='1'){
    digitalWrite(16, LOW);
  }
  else{
    digitalWrite(16, HIGH);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  pinMode(16, OUTPUT);
  client.ubidotsSubscribe("alexa","button"); //Insert the dataSource and Variable's Labels
  }

void loop() {
  // put your main code here, to run repeatedly:

  ///these are random numbers, please coment them when testing
  // real code
  h += 1.25;
  t += 2.78;

  if(!client.connected()){
      client.reconnect();
      client.ubidotsSubscribe("alexa","button"); //Insert the dataSource and Variable's Labels
      }

  client.add("Humidity", h); //Insert your variable Labels and the value to be sent
  client.add("Temperature", t);
  client.ubidotsPublish("alexa"); 

  client.loop();
  }