#include "config.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "sensors.h"
#include "led.h"

unsigned long lastSend = 0;
float currentLux = 0;

void applyLedWithBrightness() {
  float brightness = 1.0 - min(currentLux, 2000.0f) / 2000.0f;
  ledSetColor(
    (int)(lastR * brightness),
    (int)(lastG * brightness),
    (int)(lastB * brightness)
  );
}

void setup() {
  Serial.begin(115200);
  ledInit();
  sensorsInit();
  connectWiFi();
  mqttInit();
  Serial.println("ESP32 готов!");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  mqttMaintain();
  mqttHandleSubscriptions();
  applyLedWithBrightness();

  if (millis() - lastSend >= SEND_INTERVAL) {
    lastSend = millis();

    float temp = readTemperature();
    float lux  = readLight();

    if (temp != -999) mqttPublishTemp(temp);
    if (lux  != -999) {
      currentLux = lux;
      mqttPublishLight(lux);
    }
  }
}