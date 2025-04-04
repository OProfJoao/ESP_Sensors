//! ------------------------------IMPORTED LIBRARIES------------------------------
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <env.h>

//! ------------------------------FUNCTION HEADERS--------------------------------




bool connectToWifi();
bool connectToBroker();
void messageReceived(const char *topic, byte *message, unsigned int length);

//! ------------------------------DEFINED VARIABLES------------------------------

WiFiClientSecure client;
PubSubClient mqttClient(client);

const char *ledTopic = "ESP32_SENSOR/ledControl";
const char *lightTopic = "ESP32_SENSOR/lightSensor";
const char *ultrassonicTopic = "ESP32_SENSOR/ultrassonicSensor";

const char *ledTopic = "ESP32_SENSOR/ledControl";

unsigned long lastTime = 0;
unsigned long now = 0;

byte lightSensor = 34;
byte led = 2;

//! ------------------------------FUNCTIONS------------------------------

bool connectToWifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Connecting to WiFi");
  byte tentativas;
  while (WiFi.isConnected() == false && tentativas < 10)
  {
    Serial.print("Status: ");
    Serial.println(WiFi.status());
    delay(1000);
    tentativas++;
  }
  if (WiFi.isConnected())
  {
    Serial.println("Connected to WiFi");
  }
  else
  {
    Serial.println("Failed to connect to WiFi");
  }
  return WiFi.isConnected();
}

bool connectToBroker()
{
  byte tentativas;
  while (mqttClient.connected() == false && tentativas < 10)
  {
    mqttClient.setServer(BROKER, 8883);
    mqttClient.connect(MQTT_BOARD_ID, MQTT_USERNAME, MQTT_PASS);
    if (mqttClient.connected() == false)
    {
      Serial.println("Failed to connect to broker");
      delay(1000);
      tentativas++;
    }
    else
    {
      Serial.println("Connected to broker");
      mqttClient.setCallback(messageReceived);

      mqttClient.subscribe(lightTopic);
    }
    delay(1000);
    tentativas++;
  }
  return false;
}

void messageReceived(const char *topic, byte *message, unsigned int length)
{
  // Do something if receives any message
}

//! ---------------------------------MAIN LOOPS--------------------------------------

void setup() {
  client.setInsecure();
  Serial.begin(9600);
  
  connectToWifi();
  connectToBroker();

  pinMode(lightSensor, INPUT);
  pinMode(led, OUTPUT);
}

void loop()
{
  if (WiFi.isConnected() == false)
  {
    connectToWifi();
  }

  if (mqttClient.connected() == false)
  {
    connectToBroker();
  }


  mqttClient.loop();
  byte lightSensorValue = map(analogRead(lightSensor), 0, 4095, 0, 100);
  Serial.println(lightSensorValue);
  now = millis();

  String ledStatus = "Apagar";

  if (WiFi.isConnected() && mqttClient.connected())
  {
    if (now - lastTime >= 1000)
    {
      lastTime = now;
      if (lightSensorValue < 80)
      {
        ledStatus = "Acender";
      }
      else
      {
        ledStatus = "Apagar";
      }
      Serial.println(ledStatus);
      mqttClient.publish(ledTopic, ledStatus.c_str(), true);
    }
  }
  delay(100);
}
