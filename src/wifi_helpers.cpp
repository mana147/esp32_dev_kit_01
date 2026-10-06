#include "wifi_helpers.h"

String getSecurityName(wifi_auth_mode_t authMode)
{
  switch (authMode)
  {
  case WIFI_AUTH_OPEN:
    return "Open";
  case WIFI_AUTH_WEP:
    return "WEP";
  case WIFI_AUTH_WPA_PSK:
    return "WPA";
  case WIFI_AUTH_WPA2_PSK:
    return "WPA2";
  case WIFI_AUTH_WPA_WPA2_PSK:
    return "WPA/WPA2";
  case WIFI_AUTH_WPA2_ENTERPRISE:
    return "WPA2-Enterprise";
  case WIFI_AUTH_WPA3_PSK:
    return "WPA3";
  case WIFI_AUTH_WPA2_WPA3_PSK:
    return "WPA2/WPA3";
  default:
    return "Unknown";
  }
}

String getBandFromChannel(int channel)
{
  if (channel >= 1 && channel <= 14)
  {
    return "2.4GHz";
  }
  if (channel >= 36 && channel <= 165)
  {
    return "5GHz";
  }
  return "Unknown";
}

String getChannelWidthFromBand(int channel)
{
  if (channel >= 1 && channel <= 14)
  {
    return "20MHz";
  }
  if (channel >= 36 && channel <= 165)
  {
    return "80MHz";
  }
  return "20MHz";
}

String getVendorFromBssid(uint8_t *bssid)
{
  if (bssid == nullptr)
  {
    return "Unknown";
  }

  uint8_t oui[3] = {bssid[0], bssid[1], bssid[2]};

  if (oui[0] == 0x00 && oui[1] == 0x0C && oui[2] == 0x43)
    return "Cisco";
  if (oui[0] == 0x00 && oui[1] == 0x17 && oui[2] == 0x9A)
    return "Cisco";
  if (oui[0] == 0x00 && oui[1] == 0x1A && oui[2] == 0x2B)
    return "Intel";
  if (oui[0] == 0x00 && oui[1] == 0x50 && oui[2] == 0x56)
    return "VMware";
  if (oui[0] == 0x00 && oui[1] == 0x80 && oui[2] == 0x9F)
    return "Netgear";
  if (oui[0] == 0x00 && oui[1] == 0x90 && oui[2] == 0x4C)
    return "TP-Link";
  if (oui[0] == 0x00 && oui[1] == 0x1D && oui[2] == 0xAA)
    return "TP-Link";
  if (oui[0] == 0x70 && oui[1] == 0x4F && oui[2] == 0x57)
    return "TP-Link";
  if (oui[0] == 0x88 && oui[1] == 0x1F && oui[2] == 0xA1)
    return "Google";
  if (oui[0] == 0xA4 && oui[1] == 0x2B && oui[2] == 0xB0)
    return "Apple";
  if (oui[0] == 0xA8 && oui[1] == 0x40 && oui[2] == 0x41)
    return "Apple";

  return "Unknown";
}

void printNetworkTableHeader()
{
  Serial.println("---------------------------------------------------------------------------------------------------------------------------------");
  Serial.println("SSID                         | BSSID             | CH | Width  | Band   | Security         | RSSI  | Signal% | Vendor");
  Serial.println("---------------------------------------------------------------------------------------------------------------------------------");
}

void printNetworkDetails(int index)
{
  String ssid = WiFi.SSID(index);
  String bssid = WiFi.BSSIDstr(index);
  int channel = WiFi.channel(index);
  int rssi = WiFi.RSSI(index);
  int signalPercent = constrain(map(rssi, -90, -30, 0, 100), 0, 100);

  String band = getBandFromChannel(channel);
  String width = getChannelWidthFromBand(channel);
  String security = getSecurityName(WiFi.encryptionType(index));
  String vendor = getVendorFromBssid(WiFi.BSSID(index));

  Serial.printf("%-27s | %-17s | %2d | %-6s | %-6s | %-16s | %4d dBm | %3d%%    | %-12s\n",
                ssid.c_str(),
                bssid.c_str(),
                channel,
                width.c_str(),
                band.c_str(),
                security.c_str(),
                rssi,
                signalPercent,
                vendor.c_str());
}
