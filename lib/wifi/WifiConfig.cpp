#include "WifiConfig.h"
#include <WiFi.h>

const char *ssid = "Thai Thanh";
const char *password = "thaithanh2019";
constexpr int builtInLed = GPIO_NUM_2;

WiFiClient client;

void startWiFi() {
    Serial.println();
    Serial.print("Setting up Wi-Fi access point: ");
    Serial.println(ssid);

    WiFi.softAP(ssid, password);
    pinMode(builtInLed, OUTPUT);
    digitalWrite(builtInLed, HIGH);

    Serial.println("Wi-Fi access point set up.");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
}

void connectWifi() {
    while (WiFiClass::status() != WL_CONNECTED) {
        Serial.println("Connecting to Wi-Fi network...");
        WiFi.begin(ssid, password);
        delay(1000);
    }
    Serial.println("Connected to Wi-Fi network");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    pinMode(builtInLed, OUTPUT);
    digitalWrite(builtInLed, HIGH);

}
