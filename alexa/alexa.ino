/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define dht_dpin 0

/****************************************
 * Define Constants
 ****************************************/

// Yoimer's Alexa TOKEN (has to be created in order to be a fixed one)
// the ones from default in web site, will expire in 6 hours if not used.
//https://app.ubidots.com/userdata/api/
#define TOKEN "A1E-nJFpIeInVtXSpIrqa8OZ4pktloFlnl"
//#define TOKEN "A1E-kVgDgXsJGczuXn5745CwVSKFS7UqBnXBzqHzARChQzBlipec3vZbo7kx" // Mohamed's Ubidots TOKEN
#define WIFINAME "Casa" //Your SSID
#define WIFIPASS "remioy2006202" // Your Wifi Pass
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

//****topic = /v1.6/devices/alexa/redled/lv
//****topic = /v1.6/devices/alexa/greenled/lv

if (topic[20]=='g')  // check if the value is coming from greenled button
  {
  if ((char)payload[0]=='1'){
    digitalWrite(5, HIGH);  // swtich on the green led
  }
  else{
    digitalWrite(5, LOW);  // switch off the green led
  }
  //Serial.println();
}

if (topic[20]=='r')  // check if the value is coming from redled button
  {
    if ((char)payload[0]=='1'){
      digitalWrite(4, HIGH);  // switch on the red led
    }
    else{
      digitalWrite(4, LOW);  // switch off the red led
    }
  //Serial.println();
  }
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
  client.begin(callback);
  pinMode(5, OUTPUT); // green led output pin (GOIO5=D1)
  pinMode(4, OUTPUT); // red led output pin (GPIO4=D2)
  client.ubidotsSubscribe("alexa","redled"); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe("alexa","greenled"); //Insert the dataSource and Variable's Labels
  }

void loop() {
  // put your main code here, to run repeatedly:

  // read temperature and humidity
  // uncoment when testing real code
  readTemperatureAndHumidity();

  ///these are random numbers, please coment them when testing
  // real code
  // h += 1.25;
  // t += 2.78;

  if(!client.connected()){
      client.reconnect();
      client.ubidotsSubscribe("alexa","greenled"); //Insert the dataSource and Variable's Labels
      client.ubidotsSubscribe("alexa","redled"); //Insert the dataSource and Variable's Labels
  }

  client.add("Humidity", h); //Insert your variable Labels and the value to be sent
  client.add("Temperature", t);
  client.ubidotsPublish("alexa"); 
  // delay(15000);
  client.loop();
}

