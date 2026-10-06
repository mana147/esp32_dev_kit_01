#ifndef WIFI_HELPERS_H
#define WIFI_HELPERS_H

#include <Arduino.h>
#include <WiFi.h>

String getSecurityName(wifi_auth_mode_t authMode);
String getBandFromChannel(int channel);
String getChannelWidthFromBand(int channel);
String getVendorFromBssid(uint8_t *bssid);
void printNetworkTableHeader();
void printNetworkDetails(int index);

#endif
