#include "SerialView.h"
#include <OuiDb.h>
#include <WifiUtils.h>

namespace serial_view {

namespace {

const char kSeparator[] =
    "-----------------------------------------------------------------------------------------------------------";

void printHeader()
{
  Serial.println(kSeparator);
  Serial.println("SSID                         | BSSID             | CH | Band   | Security         | RSSI    | Signal% | Vendor");
  Serial.println(kSeparator);
}

void printRow(const NetworkInfo &n)
{
  char mac[18];
  snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X",
           n.bssid[0], n.bssid[1], n.bssid[2], n.bssid[3], n.bssid[4], n.bssid[5]);

  Serial.printf("%-28s | %-17s | %2u | %-6s | %-16s | %4d dBm | %3d%%    | %s\n",
                n.ssid.c_str(),
                mac,
                n.channel,
                wifi_utils::bandFromChannel(n.channel),
                WifiScanner::securityName(n.auth),
                n.rssi,
                wifi_utils::signalPercentFromRssi(n.rssi),
                oui_db::vendorFromBssid(n.bssid));
}

} // namespace

void printButtonPressed(uint8_t pin)
{
  Serial.printf("[Button] GPIO%u pressed (HIGH)\n", static_cast<unsigned int>(pin));
  Serial.println();
}

void printScanStart()
{
  Serial.println();
  Serial.println("Scanning WiFi networks...");
}

void printScanFailed()
{
  Serial.println("WiFi scan failed.");
}

void printNetworks(const WifiScanner &scanner)
{
  if (scanner.count() == 0)
  {
    Serial.println("No WiFi networks found.");
    return;
  }

  printHeader();
  NetworkInfo info;
  for (int i = 0; i < scanner.count(); ++i)
  {
    if (scanner.get(i, info))
      printRow(info);
  }
  Serial.println(kSeparator);
}

} // namespace serial_view
