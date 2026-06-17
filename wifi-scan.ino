/*
 * Projeto: SmartWeather Station IoT
 * Descrição: Monitoramento ambiental com ESP32, DHT22, TFT ILI9341, LCD 16x2.
 * Bônus Implementados: Consulta ViaCEP (Nível 3) e Previsão Min/Max (Nível 1).
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <DHT.h>

// Definições de Pinos
#define DHTPIN 13
#define DHTTYPE DHT22
#define LED_GREEN 14
#define LED_RED 27
#define BUZZER 26
#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST 4

// Instâncias
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Configurações de Rede e APIs
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Open-Meteo configurado para trazer o clima atual e a previsão diária (max/min)
const char* openMeteoURL = "https://api.open-meteo.com/v1/forecast?latitude=-8.05&longitude=-34.88&current_weather=true&daily=temperature_2m_max,temperature_2m_min&timezone=America%2FSao_Paulo";
const char* viaCepURL = "https://viacep.com.br/ws/52111160/json/";

// Variáveis Globais
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 30000; 
float localTemp = 0.0, localHum = 0.0;
float apiTemp = 0.0, apiMax = 0.0, apiMin = 0.0;
String apiTime = "N/A";
String locationName = "Buscando CEP...";
bool apiStatus = false;

const float TEMP_LIMIT = 20.0;

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  dht.begin();
  lcd.init();
  lcd.backlight();
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  
  connectWiFi();
  fetchViaCEP(); // Bônus Nível 3: Roda apenas uma vez para pegar a cidade
  
  readSensors();
  fetchWeatherAPI(); // Bônus Nível 1: Clima e Previsão
  updateDisplays();
}

void loop() {
  if (millis() - lastUpdate >= updateInterval) {
    readSensors();
    fetchWeatherAPI();
    updateDisplays();
    lastUpdate = millis();
  }
}

void connectWiFi() {
  lcd.setCursor(0, 0);
  lcd.print("Conectando Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  lcd.clear();
}

void readSensors() {
  localTemp = dht.readTemperature();
  localHum = dht.readHumidity();
  
  if (localTemp > TEMP_LIMIT) {
    tone(BUZZER, 1000, 500);
  }
}

void fetchViaCEP() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(viaCepURL);
    if (http.GET() == 200) {
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, http.getString());
      String cidade = doc["localidade"];
      String estado = doc["uf"];
      locationName = cidade + " - " + estado;
    } else {
      locationName = "CEP Nao Encontrado";
    }
    http.end();
  }
}

void fetchWeatherAPI() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(openMeteoURL);
    
    if (http.GET() == 200) {
      DynamicJsonDocument doc(2048); // Tamanho aumentado para o JSON maior
      deserializeJson(doc, http.getString());
      
      apiTemp = doc["current_weather"]["temperature"];
      String timeStr = doc["current_weather"]["time"];
      apiTime = timeStr.substring(11, 16);
      
      // Bônus Nível 1: Extraindo Max e Min do array 'daily'
      apiMax = doc["daily"]["temperature_2m_max"][0];
      apiMin = doc["daily"]["temperature_2m_min"][0];
      
      apiStatus = true;
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
    } else {
      apiStatus = false;
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
    }
    http.end();
  }
}

void updateDisplays() {
  // Atualização LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(localTemp, 1); lcd.print("C API:"); lcd.print(apiTemp, 1);
  lcd.setCursor(0, 1);
  lcd.print("Umid: "); lcd.print(localHum, 0); lcd.print("%");
  
  // Atualização TFT
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  
  tft.println(locationName); // Exibe o resultado do ViaCEP
  tft.drawLine(10, 30, 300, 30, ILI9341_WHITE);
  
  tft.setCursor(10, 40);
  tft.setTextColor(ILI9341_CYAN);
  tft.print("Temp Local: "); tft.print(localTemp, 1); tft.println(" C");
  tft.setCursor(10, 70);
  tft.print("Umid Local: "); tft.print(localHum, 0); tft.println(" %");
  
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(10, 110);
  tft.print("Temp API: "); tft.print(apiTemp, 1); tft.println(" C");
  
  tft.setCursor(10, 140);
  tft.setTextSize(1);
  tft.print("Previsao: Min "); tft.print(apiMin, 1); tft.print("C / Max "); tft.print(apiMax, 1); tft.println("C");
  
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(10, 170);
  tft.print("Ultima At.: "); tft.println(apiTime);
  
  tft.setCursor(10, 210);
  tft.print("Status: ");
  if (apiStatus) {
    tft.setTextColor(ILI9341_GREEN);
    tft.println("CONECTADO");
  } else {
    tft.setTextColor(ILI9341_RED);
    tft.println("ERRO REDE");
  }
}