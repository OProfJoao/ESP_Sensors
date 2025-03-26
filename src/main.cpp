#include <Arduino.h>
#include <esp32DHT.h>
#include <pubSubClient.h>
#include <WiFiClientSecure.h>
#include <env.h>


WiFiClientSecure client;

void connectToWifi();
void connectToBroker();

void setup()
{
  client.setInsecure();

  Serial.begin(9600);

  connectToWifi();
  connectToBroker();
}

void loop()
{

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

}