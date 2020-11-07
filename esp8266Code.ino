#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
String ifttt_key = "cvsGyWrMD6clchrP8Uz3Ow"; // webhooks key
String ifttt_event_name = "Solar_Panel_inefficiency_detected"; //webhooks event name
String value1 = ""; //value1 that you want to send when the event is triggered
SoftwareSerial NodeMCU(4,0);
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID       "Chandu"
#define WLAN_PASS       "1968chandu"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "solar_monitor"
#define AIO_KEY         "aio_xwEr52EoZVKd5ZqLZgUs9YTjLTfR"


// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// Setup a feed called 'power' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish power = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/power");
void MQTT_connect();
void setup() {
  Serial.begin(9600);
  NodeMCU.begin(9600);
  pinMode(4,INPUT);
  pinMode(0,OUTPUT);
  delay(1000);
  Serial.println(F("Adafruit MQTT demo"));
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  } 
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}
uint32_t x=0;
void loop() {
  MQTT_connect();
  // Now we can publish stuff!
  Serial.print(F("\nSending power val "));
  float pwr=0;
  if(NodeMCU.available()>0){
    pwr = NodeMCU.parseFloat();
    if(NodeMCU.read()=='\n'){
      Serial.print("Power = ");
      Serial.println(pwr);  
    }  
  }
  //if power less than threshold send notification
  if(pwr <= 0.07){
    HTTPClient http;
    value1 = String(pwr);
    http.begin("http://maker.ifttt.com/trigger/"+ifttt_event_name+"/with/key/"+ifttt_key+"?value1="+value1);
    http.GET();
    http.end();
    Serial.println("done");  
  }
  if (! power.publish(pwr)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  delay(5000);
}
// This code is taken from Arduino Examples in Arduino hub on official site
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
