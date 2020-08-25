# Предмет: Дигитална електроника

## Опис пројекта: 

Микроконтролер ЕСП8266 (NodeMCU v1.0) + DHT11 сензор температуре и влажности ваздуха са 
приказом информација на HTML страници (AJAX).

```
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
```
