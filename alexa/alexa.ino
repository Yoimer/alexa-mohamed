// #include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
// #define DHTTYPE DHT11   // DHT 11

// #define dht_dpin 0
// DHT dht(dht_dpin, DHTTYPE); 
// void setup(void)
// { 
//   dht.begin();
//   Serial.begin(9600);
//   Serial.println("Humidity and temperature\n\n");
//   delay(700);

// }
// void loop() {
//     readTemperatureAndHumidity();
// }

// ////readTemperatureAndHumidity
// float readTemperatureAndHumidity() {
//     float h = dht.readHumidity();
//     float t = dht.readTemperature();         
//     Serial.print("Current humidity = ");
//     Serial.print(h);
//     Serial.print("%  ");
//     Serial.print("temperature = ");
//     Serial.print(t); 
//     Serial.println("C  ");
//     delay(800);
// }

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "A1E-kVgDgXsJGczuXn5745CwVSKFS7UqBnXBzqHzARChQzBlipec3vZbo7kx" // Your Ubidots TOKEN
#define WIFINAME "SKY7D85E" //Your SSID
#define WIFIPASS "8121985" // Your Wifi Pass

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
  }

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      client.reconnect();
      }
  
  // Publish values to 2 different data sources
  
  client.add("stuff", 10.2); //Insert your variable Labels and the value to be sent
  client.ubidotsPublish("source1");
  client.add("stuff", 10.2);
  client.add("more-stuff", 120.2);
  client.ubidotsPublish("source2");
  client.loop();
  }
