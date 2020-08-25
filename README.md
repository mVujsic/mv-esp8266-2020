# Предмет: Дигитална електроника

## Опис пројекта: 

Микроконтролер ЕСП8266 (NodeMCU v1.0) + DHT11 сензор температуре и влажности ваздуха са 
приказом информација на HTML страници (AJAX).

## Захтевано 

``` Arduino
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
```
## Повезивање

* D1(NodeMCU v1.0) * са * DATA(DHT11) *

3.3V(NodeMCU v1.0) са VCC(DHT11)

GND(NodeMCU v1.0) са GND(DHT11)

![shema](/img/shema.png)

## UI

![UI](/img/UserInterface.png)

## Serial Monitor

![SM](/img/SerialMonitor.png)
