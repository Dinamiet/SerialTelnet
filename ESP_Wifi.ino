#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266WiFi.h> // https://github.com/espressif/arduino-esp32

#define HOST_NAME "ESP-8266"
#define PORT 23
#define BUFFER_SIZE 512


WiFiManager manager;
WiFiServer server(PORT);
WiFiClient client;

uint8_t buffer[BUFFER_SIZE];


void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);

    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP    
    
    
    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    manager.setConfigPortalBlocking(false);
    manager.setHostname(HOST_NAME);
    // wm.setConfigPortalTimeout(60);
    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    manager.autoConnect(HOST_NAME);

    server.begin();
}

void loop() {
    manager.process();

    if (server.hasClient())
    {
      client = server.accept();
      digitalWrite(LED_BUILTIN, 1);
    }

    if (Serial.available())
    {
      size_t size = Serial.read(buffer, BUFFER_SIZE);
      client.write(buffer, size);
    }

    if (client.available())
    {
      size_t size = client.read(buffer, BUFFER_SIZE);
      Serial.write(buffer, size);
    }
}
