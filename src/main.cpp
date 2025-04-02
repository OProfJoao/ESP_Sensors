#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <env.h>

/*------------------------------VARIÁVEIS------------------------------*/
WiFiClientSecure client;
PubSubClient mqttClient(client);

const char *ledTopic = "ESP32_SENSOR/ledControl";
const char *lightTopic = "ESP32_SENSOR/lightSensor";

byte lightSensor = 34;

/*------------------------------FUNÇÕES------------------------------*/

bool connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectando ao WiFi...");
  
  byte tentativas = 0;
  while (!WiFi.isConnected() && tentativas < 10) {
    delay(1000);
    tentativas++;
  }

  return WiFi.isConnected();
}

bool connectToBroker() {
  mqttClient.setServer(BROKER, 8883);

  byte tentativas = 0;
  while (!mqttClient.connected() && tentativas < 10) {
    if (mqttClient.connect("ESP32_SENSOR", MQTT_USERNAME, MQTT_PASS)) {
      Serial.println("Conectado ao broker MQTT!");
      return true;
    }
    delay(1000);
    tentativas++;
  }
  return false;
}

void setup() {
  client.setInsecure();
  Serial.begin(9600);
  
  connectToWifi();
  connectToBroker();
}

void loop() {
  if (!WiFi.isConnected()) {
    Serial.println("WiFi desconectado! Tentando reconectar...");
    connectToWifi();
  }

  if (!mqttClient.connected()) {
    Serial.println("Broker desconectado! Tentando reconectar...");
    connectToBroker();
  }


  mqttClient.loop();

  // Leitura do sensor e publicação MQTT
  int lightSensorValue = analogRead(lightSensor);
  lightSensorValue = map(lightSensorValue, 0, 4095, 0, 100);

  Serial.print("Luminosidade: ");
  Serial.println(lightSensorValue);

  // char msgBuffer[10];
  // sprintf(msgBuffer, "%d", lightSensorValue);
  // mqttClient.publish(lightTopic, msgBuffer);

  if (lightSensorValue < 50) {
    mqttClient.publish(ledTopic, (uint8_t*)"Acender", strlen("Acender"),true);
    Serial.println("Enviando: Acender");
  } else {
    mqttClient.publish(ledTopic, (uint8_t*)"Apagar", strlen("Apagar"),true);
    Serial.println("Enviando: Apagar");
  }

  delay(2000);
}
