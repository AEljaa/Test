#include <WiFi.h>
#include <WebSocketsClient.h>
#include <config.h>
#include <ArduinoJson.h>

const char *serverAddress = "ws://172.20.10.3";
int serverPort = 8080;

WebSocketsClient webSocket;

#define DEBUG_WEBSOCKETS(...) os_printf( __VA_ARGS__ )

#ifndef DEBUG_WEBSOCKETS
#define DEBUG_WEBSOCKETS(...) 
#define NODEBUG_WEBSOCKETS
#endif



void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.println("[WebSocket] Disconnected");
    break;
  case WStype_CONNECTED:
    Serial.println("[WebSocket] Connected");
    break;
  case WStype_TEXT:
    Serial.printf("[WebSocket] Message received: %s\n", payload);
    break;
  }
}

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  webSocket.begin(serverAddress, serverPort, "/");
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();

  if (!webSocket.isConnected())
  {
    Serial.println("WebSocket not connected. Reconnecting...");
    webSocket.begin(serverAddress, serverPort, "/");
  }

  // Create a JSON object
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["message"] = "Hello from ESP32!";

  // Serialize the JSON object into a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Send the JSON message to the server
  webSocket.sendPing();
  Serial.println("Ping sent");

  delay(5000);
}