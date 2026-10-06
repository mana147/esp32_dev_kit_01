#pragma once
#include <stdint.h>

namespace cfg
{

// Serial port configuration.
constexpr uint32_t kSerialBaud = 115200;
constexpr uint32_t kSerialBootDelayMs = 1000;

// LED onboard tại GPIO2, active HIGH.
constexpr uint8_t kLedPin = 2;

// Button wiring: one side to 3.3V, other side to GPIO15 (internal pull-down).
constexpr uint8_t kButtonPin = 15;
constexpr uint32_t kDebounceMs = 30;

// Set to true to re-enable WiFi scanning.
constexpr bool kEnableWifiScan = false;
constexpr uint32_t kScanIntervalMs = 5000;

// WiFi scan timeout in milliseconds.
constexpr uint32_t kWifiScanTimeoutMs = 10000; // example additional configuration for WiFi scan timeout


} // namespace cfg
