#include <WiFi.h>
#include "../lib/wifi_config/WifiConfig.h"
#include "../lib/server/ServerConfig.h"


const int ledPin = 2;

void setup() {
    startWiFi();
    startHttpServer();
}

void loop() {
    handleRequest();
}
