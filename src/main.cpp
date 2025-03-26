/*------------------------------IMPORTED LIBRARIES------------------------------*/
#include <Arduino.h>
#include <esp32DHT.h>
#include <pubSubClient.h>
#include <WiFiClientSecure.h>
#include <env.h>


/*------------------------------FUNCTION HEADERS------------------------------*/

void connectToWifi();
void connectToBroker();
void messageReceived(const char* topic, byte* message, unsigned int length);

/*------------------------------DEFINED VARIABLES------------------------------*/

WiFiClientSecure client;
PubSubClient mqttClient(client);

const char *tempUmdTopic = "ESP32_SENSOR/tempUmdSensor";
const char *lightTopic = "ESP32_SENSOR/lightSensor";
const char *ultrassonicTopic = "ESP32_SENSOR/ultrassonicSensor";

const char *ledTopic = "ESP32_SENSOR/ledControl";


byte lightSensor = 34;
byte led = 2;



/*------------------------------FUNCTIONS------------------------------*/

void connectToWifi(){
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  Serial.println("Connecting to WiFi");
  byte tentativas;
  while(WiFi.isConnected() == false && tentativas < 10){
    Serial.print("Status: "); 
    Serial.println(WiFi.status());
    delay(1000);
    tentativas ++;
  }
  Serial.println("Connected to WiFi");
}

void connectToBroker(){
  byte tentativas;
  while(mqttClient.connected() == false && tentativas < 10){
    mqttClient.setServer(BROKER,8883);
    mqttClient.connect("ESP32_SENSOR",MQTT_USERNAME,MQTT_PASS);
    if(mqttClient.connected() == false){
      Serial.println("Failed to connect to broker");
      delay(1000);
      tentativas ++;
    }else{
      Serial.println("Connected to broker");
      mqttClient.setCallback(messageReceived);

      mqttClient.subscribe(ledTopic);
    }
  }
}

void messageReceived(const char* topic, byte* message, unsigned int length){
  //Do something if receives any message
}



/*---------------------------------MAIN LOOPS--------------------------------------*/

void setup()
{
  client.setInsecure();

  Serial.begin(9600);

  connectToWifi();
  connectToBroker();

  pinMode(lightSensor,INPUT);
  pinMode(led,OUTPUT);
}

void loop()
{
  // if(WiFi.isConnected() == false){
  //   connectToWifi();
  // }
  // if(mqttClient.connected() == false){
  //   connectToBroker();
  // }
  // mqttClient.loop();
  Serial.print("Luz: ");
  byte lightSensorValue = map(analogRead(lightSensor),0,4095,0,100);
  Serial.println(lightSensorValue);
  if(lightSensorValue < 80){
    digitalWrite(led,HIGH);
  }else{
    digitalWrite(led,LOW);
  }
}