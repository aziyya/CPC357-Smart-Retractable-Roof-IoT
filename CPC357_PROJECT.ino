#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// --- WIFI & FIREBASE CONFIG ---
#define WIFI_SSID "aziy's"          
#define WIFI_PASSWORD "aziyhandsome"  
#define API_KEY "AIzaSyC6Y1_5jSQp4hLLtaUn9ApWvRm34qtClUI"
#define DATABASE_URL "cpc357-6fbdf-default-rtdb.asia-southeast1.firebasedatabase.app" 

// --- PINS: SENSORS & LED ---
const int lightSensorPin = A0;
const int rainSensorPin = D6;  
const int ledPin = D5;         

// --- PINS: MOTOR SHIELD ---
const int MOTOR_LATCH = D1; 
const int MOTOR_CLK   = D2; 
const int MOTOR_ENABLE= D3; 
const int MOTOR_DATA  = D4; 
const int PWM_M1      = D7; 
const int PWM_M2      = D8;

// --- SETTINGS & VARIABLES ---
int darkThreshold = 800; 
int motorSpeed = 700;    
int motorRegister = 0;
String roofState = "Unknown"; 
unsigned long sendDataPrevMillis = 0;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

// --- MOTOR HELPER FUNCTIONS ---
void latch_tx() {
  digitalWrite(MOTOR_LATCH, LOW);
  shiftOut(MOTOR_DATA, MOTOR_CLK, MSBFIRST, motorRegister);
  digitalWrite(MOTOR_LATCH, HIGH);
}

void stopBoth() {
  motorRegister = 0;
  latch_tx();
  analogWrite(PWM_M1, 0);
  analogWrite(PWM_M2, 0);
}

void moveClose() {
  motorRegister = 0;
  bitSet(motorRegister, 2); bitClear(motorRegister, 3); // M1 Fwd
  bitClear(motorRegister, 1); bitSet(motorRegister, 4); // M2 Bwd
  latch_tx();
  analogWrite(PWM_M1, motorSpeed);
  analogWrite(PWM_M2, motorSpeed);
}

void moveOpen() {
  motorRegister = 0;
  bitClear(motorRegister, 2); bitSet(motorRegister, 3); // M1 Bwd
  bitSet(motorRegister, 1); bitClear(motorRegister, 4); // M2 Fwd
  latch_tx();
  analogWrite(PWM_M1, motorSpeed);
  analogWrite(PWM_M2, motorSpeed);
}

void setup() {
  Serial.begin(9600);
  
  // Pin Setup
  pinMode(ledPin, OUTPUT);
  pinMode(rainSensorPin, INPUT);
  pinMode(MOTOR_LATCH, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(MOTOR_DATA, OUTPUT);
  pinMode(MOTOR_CLK, OUTPUT);
  pinMode(PWM_M1, OUTPUT);
  pinMode(PWM_M2, OUTPUT);
  digitalWrite(MOTOR_ENABLE, LOW); 

  // 1. Connect Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected!");

  // 2. Firebase Setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    signupOK = true;
    Serial.println("Firebase Signup OK");
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // --- 1. SENSOR READINGS ---
  int lightValue = analogRead(lightSensorPin);
  bool isRaining = (digitalRead(rainSensorPin) == LOW); 
  bool isDark = (lightValue > darkThreshold);

  // --- 2. LED LOGIC (Lights) ---
  if (isDark) {
    digitalWrite(ledPin, HIGH); 
  } else {
    digitalWrite(ledPin, LOW);
  }

  // --- 3. MOTOR LOGIC (Roof) ---
  if (isRaining) { 
    if (roofState != "Closed") {
      moveClose();
      delay(5000); 
      stopBoth();
      roofState = "Closed";
    }
  } else { 
    if (roofState != "Open") {
      moveOpen();
      delay(5000); 
      stopBoth();
      roofState = "Open";
    }
  }

  // --- 4. FIREBASE UPLOAD (Every 5 seconds) ---
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000)) {
    sendDataPrevMillis = millis();

    Firebase.RTDB.setInt(&fbdo, "/stadium/light", lightValue);
    Firebase.RTDB.setBool(&fbdo, "/stadium/isRaining", isRaining);
    Firebase.RTDB.setString(&fbdo, "/stadium/roofStatus", roofState);
    Firebase.RTDB.setBool(&fbdo, "/stadium/ledStatus", isDark);
    
    Serial.println("Data synced to Firebase");
  }
}