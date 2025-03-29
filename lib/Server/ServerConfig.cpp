#include "ServerConfig.h"
#include "WebServer.h"

WebServer server(80);

void startHttpServer() {
    server.begin();
    Serial.println("HTTP server started");
}

void handleRequest() {
    server.handleClient();
}