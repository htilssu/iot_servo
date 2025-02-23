#include "WifiConfig.h"
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "THTrueLove";
const char* password = "12345678";

void startWiFi() {
    Serial.begin(115200);
    delay(10);

    Serial.println();
    Serial.print("Setting up Wi-Fi access point: ");
    Serial.println(ssid);

    WiFi.softAP(ssid, password);

    Serial.println("Wi-Fi access point set up.");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
}