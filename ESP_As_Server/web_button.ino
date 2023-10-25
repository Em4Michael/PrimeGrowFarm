#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char* ssid = "MTN_4G_31E593";
const char* password = "C6E31CBF";

WebSocketsServer webSocket = WebSocketsServer(81);

const int E_motor_Down = 5;
const int E_motor_Up = 15;
const int E_Fan = 21;
const int E_Pump = 4;
const int E_pest = 18;
const int E_Light = 19;

bool state_E_motor_Down = LOW;
bool state_E_motor_Up = LOW;
bool state_E_Fan = LOW;
bool state_E_Pump = LOW;
bool state_E_pest = LOW;
bool state_E_Light = LOW;

void setup() {
  Serial.begin(115200);

  pinMode(E_motor_Down, OUTPUT);
  pinMode(E_motor_Up, OUTPUT);
  pinMode(E_Fan, OUTPUT);
  pinMode(E_Pump, OUTPUT);
  pinMode(E_pest, OUTPUT);
  pinMode(E_Light, OUTPUT);

  digitalWrite(E_motor_Down, state_E_motor_Down);
  digitalWrite(E_motor_Up, state_E_motor_Up);
  digitalWrite(E_Fan, state_E_Fan);
  digitalWrite(E_Pump, state_E_Pump);
  digitalWrite(E_pest, state_E_pest);
  digitalWrite(E_Light, state_E_Light);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();

  // Replace Serial.available with Serial.availableForWrite() to prevent buffer overflow
  if (Serial.availableForWrite()) {
    String sensorData = Serial.readStringUntil('\n');
    Serial.println(sensorData);

    // Parse JSON data
    DynamicJsonDocument jsonDocument(200);
    DeserializationError error = deserializeJson(jsonDocument, sensorData);

    if (!error) {
      String jsonString;
      serializeJson(jsonDocument, jsonString);
      webSocket.broadcastTXT(jsonString);
    } else {
      Serial.println("Error parsing JSON data");
    }
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_TEXT:
      if (strncmp((char *)payload, "toggle:", 7) == 0) {
        char pinName[20];
        strncpy(pinName, (char *)payload + 7, length - 7);
        pinName[length - 7] = '\0';

        if (strcmp(pinName, "E_motor_Down") == 0) {
          state_E_motor_Down = !state_E_motor_Down;
          digitalWrite(E_motor_Down, state_E_motor_Down);
        } else if (strcmp(pinName, "E_motor_Up") == 0) {
          state_E_motor_Up = !state_E_motor_Up;
          digitalWrite(E_motor_Up, state_E_motor_Up);
        } else if (strcmp(pinName, "E_Fan") == 0) {
          state_E_Fan = !state_E_Fan;
          digitalWrite(E_Fan, state_E_Fan);
        } else if (strcmp(pinName, "E_Pump") == 0) {
          state_E_Pump = !state_E_Pump;
          digitalWrite(E_Pump, state_E_Pump);
        } else if (strcmp(pinName, "E_pest") == 0) {
          state_E_pest = !state_E_pest;
          digitalWrite(E_pest, state_E_pest);
        } else if (strcmp(pinName, "E_Light") == 0) {
          state_E_Light = !state_E_Light;
          digitalWrite(E_Light, state_E_Light);
        } else {
          // Invalid pin name
          Serial.println("Invalid pin name");
          return;
        }

        // Send pin state back to the client
        webSocket.sendTXT(num, pinName);
      }
      break;
  }
}
