#include <WiFi.h>
#include <WebServer.h>
#include "../lib/wifi_config/WifiConfig.h"


WebServer server(80);

const int ledPin = 2; // GPIO pin where the LED is connected

void handleRoot() {
    server.send(200, "text/html", R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>ESP32 Web Server</title>
            <style>
                body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
                h1 { color: #333; }
                button { padding: 10px 20px; font-size: 16px; margin: 10px; cursor: pointer; }
                .on { background-color: #4CAF50; color: white; }
                .off { background-color: #f44336; color: white; }
            </style>
        </head>
        <body>
            <h1>ESP32 Web Server</h1>
            <p><a href="/LED_ON"><button class="on">Turn LED ON</button></a></p>
            <p><a href="/LED_OFF"><button class="off">Turn LED OFF</button></a></p>
        </body>
        </html>
    )rawliteral");
}

void handleLEDOn() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/html", R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>ESP32 Web Server</title>
            <style>
                body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
                h1 { color: #4CAF50; }
                button { padding: 10px 20px; font-size: 16px; margin: 10px; cursor: pointer; }
                .back { background-color: #333; color: white; }
            </style>
        </head>
        <body>
            <h1>LED is ON</h1>
            <p><a href="/"><button class="back">Go Back</button></a></p>
        </body>
        </html>
    )rawliteral");
}

void handleLEDOff() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/html", R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>ESP32 Web Server</title>
            <style>
                body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
                h1 { color: #f44336; }
                button { padding: 10px 20px; font-size: 16px; margin: 10px; cursor: pointer; }
                .back { background-color: #333; color: white; }
            </style>
        </head>
        <body>
            <h1>LED is OFF</h1>
            <p><a href="/"><button class="back">Go Back</button></a></p>
        </body>
        </html>
    )rawliteral");
}

void setup() {
    startWiFi();

    server.on("/", handleRoot);
    server.on("/LED_ON", handleLEDOn);
    server.on("/LED_OFF", handleLEDOff);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
