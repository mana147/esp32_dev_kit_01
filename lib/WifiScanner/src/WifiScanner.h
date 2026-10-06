#pragma once
#include <Arduino.h>
#include <WiFi.h>

struct NetworkInfo
{
  String ssid;
  uint8_t bssid[6];
  int8_t rssi;
  uint8_t channel;
  wifi_auth_mode_t auth;
};

enum class ScanState
{
  Idle,    // no scan in progress
  Running, // scan in progress
  Done,    // results are available
  Failed   // scan failed
};

class WifiScanner
{
public:
  void begin();
  void start();     // start an async scan (ignored while one is running)
  ScanState poll(); // call from loop()
  int count() const { return _count; }
  bool get(int index, NetworkInfo &out) const;
  void release(); // free scan results

  static const char *securityName(wifi_auth_mode_t auth);
  
private:
  bool _running = false;
  int _count = 0;
  
};
