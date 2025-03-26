#include <Arduino.h>
#include <esp32DHT.h>
#include <pubSubClient.h>
#include <WiFiClientSecure.h>
#include <env.h>


WiFiClientSecure client;
PubSubClient mqttClient(client);

void connectToWifi();
void connectToBroker();
void messageReceived(const char* topic, byte* message, unsigned int length);

void setup()
{
  client.setInsecure();

  Serial.begin(9600);

  connectToWifi();
  connectToBroker();
}

void loop()
{
  if(WiFi.isConnected() == false){
    connectToWifi();
  }
  if(mqttClient.connected() == false){
    connectToBroker();
  }
  mqttClient.loop();
}



void connectToWifi(){
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  Serial.println("Connecting to WiFi");
  while(WiFi.isConnected() == false){
    Serial.print("Status: "); 
    Serial.println(WiFi.status());
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void connectToBroker(){
  while(mqttClient.connected() == false){
    mqttClient.setServer(BROKER,8883);
    mqttClient.connect("ESP32_SENSOR",MQTT_USERNAME,MQTT_PASS);
    if(mqttClient.connected() == false){
      Serial.println("Failed to connect to broker");
      delay(1000);
    }else{
      Serial.println("Connected to broker");
      mqttClient.setCallback(messageReceived);
    }
  }
}

void messageReceived(const char* topic, byte* message, unsigned int length){
  //Do something if receives any message
}