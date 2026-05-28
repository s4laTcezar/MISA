#pragma once
#include <WiFi.h>
#include "config.h"

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Pripájanie k WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi pripojené! IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi nedostupné — opakujem o 5 sek");
    delay(5000);
  }
}