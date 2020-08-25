#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Одговарајући параметри мреже
const char* ssid = "ZTE_H168N87DF8C";
const char* password = "***********";

#define DHTPIN 5     // Пин 5 одговара пину Д1 ЕСП8266

#define DHTTYPE    DHT11     // DHT 11 сензор 


DHT dht(DHTPIN, DHTTYPE);

// Иницијализација температуре и влажности ваздуха
float t = 0.0;
float h = 0.0;

AsyncWebServer server(80);

unsigned long previousMillis = 0;    // Битно за очитавања

// Интервал очитавања са сензора је 10 секунди
const long interval = 5000;  

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1" charset="UTF-8">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
  </style>
</head>
<body>
  <h2>NodeMCU v1.0 асинхрони веб-сервер</h2>
  <br>
  <br>
  <p>
    <i class="fas fa-thermometer-quarter" style="color:#059e8a;"></i> 
    <span class="dht-labels">Температура</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Влажност ваздуха</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 5000 ) ;
</script>
</html>)rawliteral";

// Процесор врши рендеровање хтмл кода у програмској меморији.
String processor(const String& var){
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

void setup(){
  // Дебагер
  Serial.begin(115200);
  dht.begin();
  
  // Повезивање на локалну мрежу
  WiFi.begin(ssid, password);
  Serial.println("Успостављање конекције");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED){
    Serial.println();
    Serial.print("IP адреса сервера:");
    Serial.println(WiFi.localIP());
  }

  // BackEnd  мини-веб-сервера
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  // Покретање сервера
  server.begin();
}
 
void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // На сваких 5 секунди се врши очитавање са сензора.
    previousMillis = currentMillis;
    
    // Очитавање температуре
    float newT = dht.readTemperature();
 
    if (isnan(newT)) {
      Serial.println("Погрешно повезани сензор ДХТ11!");
    }
    else {
      t = newT;
      Serial.print("Тренутна вредност температуре ваздуха:");
      Serial.println(t);
    }
    // Очитавање влажности ваздуха
    float newH = dht.readHumidity();
     
    if (isnan(newH)) {
      Serial.println("Грешка у читању");
    }
    else {
      h = newH;
      Serial.print("Тренутна вредност влажности ваздуха:");
      Serial.println(h);
      Serial.println("=====================");
    }
  }
  
}
