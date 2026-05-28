# ESP32 Sensor Monitor
Meranie fyzikálnych veličín s bezdrôtovým prenosom

## 1. Architektúra systému

Projekt meria teplotu (DS18B20) a intenzitu svetla (BH1750) pomocou ESP32 a odosiela dáta cez WiFi na Adafruit IO (MQTT over TLS). Farba RGB LED je ovládaná automaticky podľa teploty cez Adafruit IO Actions. Jas LED je ovládaný lokálne na ESP32 podľa osvetlenosti.

```
[DS18B20] ──┐
            ├──► [ESP32] ──── WiFi / MQTT TLS ────► [Adafruit IO]
[BH1750]  ──┘                                            │
                                                    Dashboard + Actions
[RGB LED] ◄─────────── MQTT subscribe ───────────────────┘
```

## 2. Hardvér

| Komponent | Popis | Protokol |
|---|---|---|
| ESP32 NodeMCU 38-pin (CP2102) | Hlavná platforma s WiFi | — |
| DS18B20 | Teplotný senzor ±0.5°C, rozsah −55 až +125°C | 1-Wire |
| BH1750 | Senzor intenzity svetla 0–65535 lx | I2C |
| RGB LED modul | Vizuálna spätná väzba, PWM riadenie | PWM |
| Rezistor 4.7 kΩ | Pull-up pre DS18B20 dátový pin | — |

## 3. Schéma zapojenia

### DS18B20

| Nožička DS18B20 | ESP32 pin | Poznámka |
|---|---|---|
| GND (ľavá) | GND | — |
| DATA (stred) | GPIO 4 | Pull-up 4.7 kΩ medzi DATA a 3.3V |
| VCC (pravá) | 3.3V | — |

### BH1750

| Pin BH1750 | ESP32 pin | Poznámka |
|---|---|---|
| VCC | 3.3V | — |
| GND | GND | — |
| SDA | GPIO 21 | I2C dáta (predvolené) |
| SCL | GPIO 22 | I2C hodiny (predvolené) |
| ADDR | GND | I2C adresa 0x23 |

### RGB LED modul

| Pin LED | ESP32 pin | Poznámka |
|---|---|---|
| R | GPIO 25 | Červený kanál — PWM |
| G | GPIO 26 | Zelený kanál — PWM |
| B | GPIO 27 | Modrý kanál — PWM |
| GND (–) | GND | Rezistory sú integrované v module |

## 4. Softvér

### Štruktúra repozitára

```
/
├── TestESP/
│   ├── TestESP.ino         # Hlavný sketch — setup() a loop()
│   ├── config.example.h    # Šablóna konfigurácie (bez hesiel)
│   ├── wifi_manager.h      # Pripojenie WiFi + automatický reconnect
│   ├── mqtt_manager.h      # Adafruit IO MQTT — publish + subscribe + TLS
│   ├── sensors.h           # Čítanie DS18B20 a BH1750 s ošetrením chýb
│   └── led.h               # PWM ovládanie RGB LED
├── .gitignore
└── README.md
```

### Konfigurácia

Skopírujte `config.example.h` do `config.h` a vyplňte:

```cpp
#define WIFI_SSID      "nazov_siete"
#define WIFI_PASSWORD  "heslo"
#define AIO_USERNAME   "vas_username"
#define AIO_KEY        "vas_aio_key"
```

## 5. Adafruit IO

### Feedy (skupina MEMS)

| Feed | Typ | Jednotka | Smer |
|---|---|---|---|
| mems.temperature | float | °C | ESP32 → Adafruit IO |
| mems.light | float | lx | ESP32 → Adafruit IO |
| mems.led-red | int 0–255 | — | Adafruit IO → ESP32 |
| mems.led-green | int 0–255 | — | Adafruit IO → ESP32 |
| mems.led-blue | int 0–255 | — | Adafruit IO → ESP32 |

### Actions

| Action | Vzorec | Výsledok |
|---|---|---|
| led-red | Round(Map(temp, 0→30, 0→255)) | Studená = 0, Teplá = 255 |
| led-green | Round(Map(temp, 0→30, 50→0)) | Trochu zelenej v strede |
| led-blue | Round(Map(temp, 0→30, 255→0)) | Studená = 255, Teplá = 0 |

## 6. Odôvodnenie výberu

| Rozhodnutie | Dôvod |
|---|---|
| ESP32 namiesto Arduino Uno | Integrovaný WiFi, dostatok RAM pre TLS stack, podpora I2C aj 1-Wire na ľubovoľných GPIO |
| DS18B20 namiesto NTC termistora | Digitálny výstup, presnosť ±0.5°C, nevyžaduje kalibráciu ani ADC |
| BH1750 namiesto fotoodporu | Výstup priamo v luxoch (0–65535 lx), nevyžaduje ADC ani kalibráciu |
| MQTT over TLS (port 8883) | Odľahčený protokol pre IoT, šifrovaný prenos, podpora publish a subscribe |
| Adafruit IO | Bezplatné, vstavaný dashboard, história dát s časovou pečiatkou, verejný prístup |
| Arduino IDE 2 | Rozsiahla komunita, dostupné knižnice pre všetky komponenty, ladenie cez Serial Monitor |
| Perióda 15 sekúnd | Free tier limit 30 správ/minútu — 2 feedy × bezpečný interval = 15 sekúnd |
| Jas lokálne na ESP32 | Adafruit IO Actions nepodporuje násobenie dvoch feedov súčasne |

## 7. Inštalácia

### Arduino IDE

1. Stiahnite [Arduino IDE 2](https://www.arduino.cc/en/software)
2. Pridajte ESP32 boards URL v `File → Preferences`:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. `Tools → Board → Boards Manager` → nainštalujte **esp32 by Espressif**
4. Nainštalujte knižnice cez `Manage Libraries`:
   - `BH1750` od Christopher Laws
   - `DallasTemperature` od Miles Burton
   - `OneWire` od Paul Stoffregen
   - `Adafruit MQTT Library` od Adafruit

### Nahratie do ESP32

1. `Tools → Board → ESP32 Dev Module`
2. Vyberte správny COM port (Silicon Labs CP210x)
3. Nahrajte kód tlačidlom →

## 8. Dashboard

Verejný dashboard:

```
https://io.adafruit.com/s4lat/dashboards/misa
```

| Blok | Feed | Popis |
|---|---|---|
| Line Chart | mems.temperature | História teploty za 24 hodín (°C) |
| Line Chart | mems.light | História osvetlenosti za 24 hodín (lx) |
| Gauge | mems.temperature | Aktuálna teplota s časovou pečiatkou |
| Gauge | mems.light | Aktuálna osvetlenosť s časovou pečiatkou |
