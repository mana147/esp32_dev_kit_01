#include <Arduino.h>
#include <WiFi.h>
#include "wifi_helpers.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(100);
}

void loop() {
  Serial.println();
  Serial.println("Scanning WiFi networks...\n");

  int networkCount = WiFi.scanNetworks(false, false);

  if (networkCount == 0) {
    Serial.println("No WiFi networks found.");
  } else {
    printNetworkTableHeader();

    for (int i = 0; i < networkCount; ++i) {
      printNetworkDetails(i);
    }

    Serial.println("---------------------------------------------------------------------------------------------------------------------------------");
  }

  WiFi.scanDelete();
  delay(5000);
}