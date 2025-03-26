#include <ESP32Servo.h>
#include "../lib/wifi/WifiConfig.h"
#include "../lib/server/ServerConfig.h"

constexpr int servoPin = GPIO_NUM_21;
Servo myServo;

void setup() {
    Serial.begin(115200);
    connectWifi();
    // startWiFi();
    startHttpServer();

    myServo.attach(servoPin);
    myServo.write(0);
}

void loop() {
    handleRequest();
}