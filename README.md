# 🏟️ Smart Retractable Roof Court Monitoring System

An IoT-based automation system for smart cities, designed to manage sport court roofing and lighting based on real-time environmental conditions. This project supports **UN Sustainable Development Goal (SDG) 11: Sustainable Cities and Communities**.

## 🚀 Features
* **Automatic Roof Control:** Open or close a sport court roof according to weather condition. 
* **Smart Lighting:** LED logic based on Light sensor thresholds.
* **Real-time Monitoring:** Data synced to database and shown in Dashboard.
* **Weather Adaptive:** Prevents infrastructure damage and optimizes energy use.

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP8266 (NodeMCU)
* **Motor Driver:** L293D Motor Shield
* **External Power:** 4x 1.5V Batteries
* **Actuators:**
  - 2x 3V-6V Dual Axis TT Gear Motors
  - 1x High-brightness LED
* **Sensors:**
  - LDR (Light Dependent Resistor)
  - Raindrop Sensor Module

## 💻 Software Requirements
* Arduino IDE
* Visual Studio Code
* Firebase Realtime Database

* * ## 💻 Libraries in Arduino IDE
* **esp8266:** by ESP8266 Community
* **Firebase ESP8266:** by Mobizt
* **Arduino JSON:** by Benoit Blanchon  

## ⚙️ Installation and Setup

### 1. Firebase Configuration
1.  Go to [Firebase Console](https://console.firebase.google.com/).
2.  Create a project and navigate to **Realtime Database**.
3.  Copy your **Database URL** and **Web API Key**.

### 2. Software Setup
1.  Open Arduino IDE and install the required libraries listed above.
2.  Go to **Tools > Board > ESP8266 Boards** and select **NodeMCU 1.0 (ESP-12E Module)**.
3.  Open the provided `.ino` source code.
4.  Replace placeholders with your credentials:
    ```cpp
    #define WIFI_SSID "Your_Network_Name"
    #define WIFI_PASSWORD "Your_Password"
    #define API_KEY "Your_Firebase_API_Key"
    #define DATABASE_URL "Your_Project_URL"
    ```
5.  Connect your ESP8266 via USB and click **Upload**.

### 3. Dashboard Setup
1.  Open Visual Studio Code and open the downloaded project folder.
2.  Open terminal and run following command:
    ```cpp
       # Create the React app
       npx create-react-app stadium-dashboard

       # Navigate into the folder
       cd stadium-dashboard

       # Install Firebase SDK
       npm install firebase
       ```
4.  Configure the Firebase credential in _firebase.js_ file in _src/_ folder.
5.  Start the local server using command _npm start_.

## 🌍 SDG 11 Impact Analysis
This project addresses **Target 11.7** (Universal access to public spaces) and **Target 11.b** (Disaster resilience).
* **Resilience:** Protects public sports infrastructure from flash floods/heavy rain.
* **Efficiency:** Reduces energy consumption by utilizing natural light when available.
* **Inclusion:** Ensures community spaces remain usable regardless of weather conditions.
