#pragma once
#include <Wire.h>
#include <BH1750.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

BH1750 lightMeter;
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature tempSensor(&oneWire);

void sensorsInit() {
  Wire.begin();

  if (!lightMeter.begin()) {
    Serial.println("CHYBA: BH1750 nenájdený! Skontrolujte zapojenie SDA/SCL.");
  } else {
    Serial.println("BH1750 inicializovaný.");
  }

  tempSensor.begin();
  if (tempSensor.getDeviceCount() == 0) {
    Serial.println("CHYBA: DS18B20 nenájdený! Skontrolujte zapojenie a odpor 4.7kΩ.");
  } else {
    Serial.println("DS18B20 inicializovaný. Zariadení: " + String(tempSensor.getDeviceCount()));
  }
}

// Vráti teplotu v °C alebo -999 pri chybe
float readTemperature() {
  tempSensor.requestTemperatures();
  float temp = tempSensor.getTempCByIndex(0);
  if (temp == DEVICE_DISCONNECTED_C) {
    Serial.println("CHYBA: DS18B20 neodpovedá!");
    return -999;
  }
  return temp;
}

// Vráti osvetlenosť v lx alebo -999 pri chybe
float readLight() {
  float lux = lightMeter.readLightLevel();
  if (lux < 0) {
    Serial.println("CHYBA: BH1750 neodpovedá!");
    return -999;
  }
  return lux;
}