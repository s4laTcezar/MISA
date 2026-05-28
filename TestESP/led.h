#pragma once
#include <Arduino.h>
#include "config.h"

static int _r = 0, _g = 0, _b = 0;

void ledInit() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  analogWrite(PIN_R, 0);
  analogWrite(PIN_G, 0);
  analogWrite(PIN_B, 0);
}

// Nastavenie jednotlivých kanálov RGB
void ledSetR(int val) {
  _r = constrain(val, 0, 255);
  analogWrite(PIN_R, _r);
  Serial.println("LED R = " + String(_r));
}

void ledSetG(int val) {
  _g = constrain(val, 0, 255);
  analogWrite(PIN_G, _g);
  Serial.println("LED G = " + String(_g));
}

void ledSetB(int val) {
  _b = constrain(val, 0, 255);
  analogWrite(PIN_B, _b);
  Serial.println("LED B = " + String(_b));
}

// Nastavenie celej farby naraz
void ledSetColor(int r, int g, int b) {
  ledSetR(r);
  ledSetG(g);
  ledSetB(b);
}