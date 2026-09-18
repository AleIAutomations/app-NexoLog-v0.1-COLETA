#include <Arduino.h>
#include "ESP32SensorsAccel.hpp"
#include "ESP32SensorsAmbiente.hpp"
#include "ESP32SensorsDistancia.hpp"
#include <Wifi.h>
#include <WifiClient.h>
#include <PubSubClient.h>
#include <Arduinojson.h>

#define DHT_PIN 4
#define DHT_MODEL DHT22
#define TRIG_PIN 19
#define ECHO_PIN 18
#define SCL_PIN 23
#define SDA_PIN 22

// credenciais WIFI
const char" WIFI-SSID = "Wokwi-GUEST";
const char" WIFI-PASSWORD = "";

// Conexao MQTT
#define MQTT_SERVER "host.wokwi.internal"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "Ale"
#define MQTT_PUB_TOPIC "FIAPIoT/nexolog/equipe01/dados"

WifiClient wifiClient;
PubSubClient mqttClient(wifiCliente);

const uint32_t INTERVALO_RECONEXAO = 5000;
uint32_t ultimatentativaWifi = 0;
uint32_t ultimaTentativaMQTT

//Intervalo de coleta
const uint32_t INTERVALO_COLETA = 2500;
uint32_t tempoAnterior = 0;

void conectarwifi() {
  Wifi.mode(WIFI_STA);
  Wifi.begin(WIFI_SSID, WIFI_PASSWORD);
  Wifi.serSleep(false);
}

void conectarMQTT() {
  if (mqttClient.connect(MQTT_CLIENT_ID))
  Serial.printin("[MQTT]")
}

void setup() {
  Serial.begin(115200);
  ESP32Sensors::Ambiente::inicializar(DHT_PIN, DHT_MODEL);
  ESP32Sensors::Distancia::inicializar(TRIG_PIN, ECHO_PIN);
  ESP32Sensors::Accel::inicializar(SCL_PIN, SDA_PIN);

  conectarWifi();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);


  Serial.println("temp,umid,dist,movimentacao");
}

void loop() {
   if (millis() - tempoAnterior >= INTERVALO_COLETA) {
    tempoAnterior = millis();

    ESP32Sensors::Ambiente::AMBIENTE amb = ESP32Sensors::Ambiente::medirAmbiente();
    ESP32Sensors::Distancia::DISTANCIA dist = ESP32Sensors::Distancia::medirDistancia();
    AccelData accel = ESP32Sensors::Accel::medirAccel();
    float movimentacao = ESP32Sensors::Accel::medirMovimentacao(accel);

    Serial.printf("%.1f,%.1f,%.1f,%.2f\r\n", amb.temp, amb.umid, dist.cm, movimentacao);
   }
}
