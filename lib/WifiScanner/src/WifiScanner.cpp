#include "WifiScanner.h"

void WifiScanner::begin()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(100);
}

void WifiScanner::start()
{
  if (_running)
    return;

  release();
  WiFi.scanNetworks(true, false);
  _running = true;
}

ScanState WifiScanner::poll()
{
  if (!_running)
    return ScanState::Idle;

  const int result = WiFi.scanComplete();
  if (result == WIFI_SCAN_RUNNING)
    return ScanState::Running;

  _running = false;
  if (result < 0)
    return ScanState::Failed;

  _count = result;
  return ScanState::Done;
}

bool WifiScanner::get(int index, NetworkInfo &out) const
{
  if (index < 0 || index >= _count)
    return false;

  out.ssid = WiFi.SSID(index);
  const uint8_t *bssid = WiFi.BSSID(index);
  if (bssid != nullptr)
    memcpy(out.bssid, bssid, sizeof(out.bssid));
  else
    memset(out.bssid, 0, sizeof(out.bssid));
  out.rssi = WiFi.RSSI(index);
  out.channel = WiFi.channel(index);
  out.auth = WiFi.encryptionType(index);
  return true;
}

void WifiScanner::release()
{
  WiFi.scanDelete();
  _count = 0;
}

const char *WifiScanner::securityName(wifi_auth_mode_t auth)
{
  switch (auth)
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
