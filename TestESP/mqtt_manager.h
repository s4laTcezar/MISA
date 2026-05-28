#pragma once
#include <WiFiClientSecure.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "config.h"
#include "led.h"

WiFiClientSecure _tlsClient;
Adafruit_MQTT_Client mqtt(&_tlsClient, AIO_SERVER, AIO_PORT, AIO_USERNAME, AIO_KEY);

// Publikovanie nameraných hodnôt
Adafruit_MQTT_Publish feedTemp  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mems.temperature");
Adafruit_MQTT_Publish feedLight = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mems.light");

// Odber RGB hodnôt — vypočítava Adafruit IO Actions podľa teploty
Adafruit_MQTT_Subscribe feedRed   = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/mems.led-red");
Adafruit_MQTT_Subscribe feedGreen = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/mems.led-green");
Adafruit_MQTT_Subscribe feedBlue  = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/mems.led-blue");

// Posledné prijaté hodnoty farieb — jas sa aplikuje lokálne v loop()
int lastR = 0, lastG = 0, lastB = 0;

void mqttInit() {
  _tlsClient.setInsecure();
  mqtt.subscribe(&feedRed);
  mqtt.subscribe(&feedGreen);
  mqtt.subscribe(&feedBlue);
}

void mqttMaintain() {
  if (mqtt.connected()) {
    mqtt.ping();
    return;
  }

  Serial.print("Pripájanie k Adafruit IO...");
  int8_t ret;
  int attempts = 0;

  while ((ret = mqtt.connect()) != 0 && attempts < 5) {
    Serial.println(" Chyba: " + String(mqtt.connectErrorString(ret)));
    mqtt.disconnect();
    delay(3000);
    attempts++;
  }

  if (mqtt.connected()) {
    Serial.println(" pripojený!");
  } else {
    Serial.println(" nepodarilo sa pripojiť.");
  }
}

void mqttHandleSubscriptions() {
  Adafruit_MQTT_Subscribe* sub;
  while ((sub = mqtt.readSubscription(100))) {
    if (sub == &feedRed) {
      lastR = atoi((char*)feedRed.lastread);
      Serial.println("Prijaté LED R = " + String(lastR));
    }
    if (sub == &feedGreen) {
      lastG = atoi((char*)feedGreen.lastread);
      Serial.println("Prijaté LED G = " + String(lastG));
    }
    if (sub == &feedBlue) {
      lastB = atoi((char*)feedBlue.lastread);
      Serial.println("Prijaté LED B = " + String(lastB));
    }
  }
}

void mqttPublishTemp(float temp) {
  if (feedTemp.publish(temp)) {
    Serial.println("Teplota odoslaná: " + String(temp, 1) + " °C");
  } else {
    Serial.println("Chyba odosielania teploty");
  }
}

void mqttPublishLight(float lux) {
  if (feedLight.publish(lux)) {
    Serial.println("Osvetlenosť odoslaná: " + String(lux, 1) + " lx");
  } else {
    Serial.println("Chyba odosielania osvetlenosti");
  }
}