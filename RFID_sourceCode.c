#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <RTClib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>  // Added for proper JSON handling

// Pin definitions
#define RST_PIN D3        // Changed to ESP8266 pin
#define SS_PIN D4         // Changed to ESP8266 pin
#define SD_CS_PIN D8      // Changed to ESP8266 pin

// Network configuration
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* server = "your_api_endpoint";
const int port = 80;  // your_api_port

// Initialize objects
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;
WiFiClient client;

void setup() {
  Serial.begin(115200);  // Higher baud rate for ESP8266
  Wire.begin();          // Initialize I2C bus
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();           // Changed from lcd.begin()
  lcd.backlight();      // Turn on backlight
  
  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    lcd.clear();
    lcd.print("SD Init Failed");
    while (1);
  }
  
  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC initialization failed!");
    lcd.clear();
    lcd.print("RTC Init Failed");
    while (1);
  }
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  lcd.clear();
  lcd.print("Connecting WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  //show wifi status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    lcd.clear();
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed!");
    lcd.clear();
    lcd.print("WiFi Failed");
  }
  
  delay(2000);
  lcd.clear();
  lcd.print("Ready to Scan");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }
  
  // Get card UID
  String cardUID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      cardUID += "0";
    }
    cardUID += String(rfid.uid.uidByte[i], HEX);
  }
  cardUID.toUpperCase();
  
  // Get current time
  DateTime now = rtc.now();
  char timestamp[25];
  sprintf(timestamp, "%04d/%02d/%02d %02d:%02d:%02d", now.year(), now.month(), now.day(),now.hour(), now.minute(), now.second());
  
         
  
  // Log to SD card
  logToSD(cardUID, timestamp);
  
  // Send to server
  sendToServer(cardUID, timestamp);
  
  // Display feedback
  lcd.clear();
  lcd.print("Card: " + cardUID);
  lcd.setCursor(0, 1);
  lcd.print("Logged");
  
  delay(2000);
  lcd.clear();
  lcd.print("Ready to Scan");
  
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

//function to log data onto the SD
void logToSD(String uid, const char* timestamp) {
  File dataFile = SD.open("log.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(timestamp);
    dataFile.print(",");
    dataFile.println(uid);
    dataFile.close();
    Serial.println("Logged to SD: " + uid);
  } else {
    Serial.println("SD write failed");
  }
}

//fuction to send data to the server for display on the web app
void sendToServer(String uid, const char* timestamp) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }
  
  StaticJsonDocument<200> doc;
  doc["uid"] = uid;
  doc["timestamp"] = timestamp;
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  if (client.connect(server, port)) {
    client.println("POST /your_mongodb_endpoint HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(jsonString.length());
    client.println();
    client.println(jsonString);
    
    // Wait for response with timeout
    unsigned long timeout = millis();
    while (client.connected() && millis() - timeout < 5000) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    
    client.stop();
    Serial.println("Data sent to server");
  } else {
    Serial.println("Server connection failed");
  }
}
