#pragma once

// ═══════════════════════════════════════════════
// WiFi — skopírujte do config.h a vyplňte
// ═══════════════════════════════════════════════
#define WIFI_SSID       "NAZOV_VASEJ_SIETE"
#define WIFI_PASSWORD   "HESLO_VASEJ_SIETE"

// ═══════════════════════════════════════════════
// Adafruit IO — io.adafruit.com → My Key
// ═══════════════════════════════════════════════
#define AIO_SERVER      "io.adafruit.com"
#define AIO_PORT        8883              // MQTT over TLS — šifrovaný prenos
#define AIO_USERNAME    "VAS_AIO_USERNAME"
#define AIO_KEY         "VAS_AIO_KLIC"

// ═══════════════════════════════════════════════
// Piny ESP32
// ═══════════════════════════════════════════════
#define PIN_R           25  // Červený kanál RGB LED
#define PIN_G           26  // Zelený kanál RGB LED
#define PIN_B           27  // Modrý kanál RGB LED
#define ONE_WIRE_PIN    4   // Dátový pin DS18B20

// ═══════════════════════════════════════════════
// Perióda odosielania dát (ms)
// Minimum 15000 — limit Adafruit IO (free tier: 30 správ/minútu)
// 2 feedy × odoslanie = bezpečný interval 15 sekúnd
// ═══════════════════════════════════════════════
#define SEND_INTERVAL   15000