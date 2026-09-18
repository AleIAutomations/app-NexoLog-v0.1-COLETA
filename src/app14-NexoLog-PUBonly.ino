#include <Arduino.h>
#include "ESP32SensorsAccel.hpp"
#include "ESP32SensorsAmbiente.hpp"
#include "ESP32SensorsDistancia.hpp"

#define DHT_PIN 4
#define DHT_MODEL DHT22
#define TRIG_PIN 19
#define ECHO_PIN 18
#define SCL_PIN 23
#define SDA_PIN 22

//Intervalo de coleta
const uint32_t INTERVALO_COLETA = 2500;
uint32_t tempoAnterior = 0;

void setup() {
  Serial.begin(115200);
  ESP32Sensors::Ambiente::inicializar(DHT_PIN, DHT_MODEL);
  ESP32Sensors::Distancia::inicializar(TRIG_PIN, ECHO_PIN);
  ESP32Sensors::Accel::inicializar(SCL_PIN, SDA_PIN);

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
