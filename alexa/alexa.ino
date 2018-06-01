/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define dht_dpin 0

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "A1E-kVgDgXsJGczuXn5745CwVSKFS7UqBnXBzqHzARChQzBlipec3vZbo7kx" // Your Ubidots TOKEN
#define WIFINAME "SKY7D85E" //Your SSID
#define WIFIPASS "8121985" // Your Wifi Pass
#define DHTTYPE DHT11   // DHT 11
// global values for humidity and temperature
float h = -1.0;
float t = -1.0;

Ubidots client(TOKEN);
DHT dht(dht_dpin, DHTTYPE); 
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
  Serial.println();
}

////readTemperatureAndHumidity
float readTemperatureAndHumidity() {
    h = dht.readHumidity();
    t = dht.readTemperature();
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    delay(800);
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  dht.begin();
  Serial.println("Humidity and temperature\n\n");
  delay(700);
  client.begin(callback);
  }

void loop() {
  // read temperature and humidity
  readTemperatureAndHumidity();
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      client.reconnect();
      }
  
  // Publish values to 2 different data sources
  client.add("stuff", h); //Insert your variable Labels and the value to be sent
  client.ubidotsPublish("source1");
  client.add("stuff", t);
  client.add("more-stuff", 120.2);
  client.ubidotsPublish("source2");
  client.loop();
  }
