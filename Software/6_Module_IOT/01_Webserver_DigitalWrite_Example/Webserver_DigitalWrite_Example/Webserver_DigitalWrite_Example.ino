
/**
 * @file 01_DigitalWrite_Example.ino
 * @author Mrityunjai Kumar (mrityunjai@sincgrid.com)
 * @brief
 * @version 0.1
 * @date 2022-09-20
 *
 * Copyright Sincgrid LLP
 *
 * Problem statement:- Glow led when button is pressed, otherwise off
 *
 */

#include <Arduino.h>

#define LED_PIN 2
#define BUTTON_PIN 4
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "ECE";
const char* password = "12345678";

WebServer server(80);

const int led = 2;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
    });

  server.on("/on", []() {
    server.send(200, "text/plain", "led on");
    digitalWrite(led, HIGH);
    });

  server.on("/off", []() {
    server.send(200, "text/plain", "led on");
    digitalWrite(led, LOW);
    });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
