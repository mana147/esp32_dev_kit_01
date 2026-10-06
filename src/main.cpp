#include "config.h"
#include <Arduino.h>
#include <Button.h>
#include <Led.h>
#include <SerialView.h>
#include <WifiScanner.h>

namespace
{
WifiScanner wifi_scanner; 
Button button(cfg::kButtonPin, cfg::kDebounceMs);
Led led(cfg::kLedPin);
uint32_t lastScanMs = 0;
} // namespace

void setup()
{
  Serial.begin(cfg::kSerialBaud); // note : 115200
  delay(cfg::kSerialBootDelayMs); // Wait for the serial port to initialize.

  // Initialize the button.
  button.begin();

  // Initialize the LED.
  led.begin();

  // Initialize the WiFi scanner if enabled.
  if (cfg::kEnableWifiScan)
  {
    wifi_scanner.begin();
  }

  // Scan immediately on boot.
  lastScanMs = millis() - cfg::kScanIntervalMs;
}

void loop()
{
  // Cập nhật trạng thái nút trước khi đọc sự kiện nhấn.
  button.update();

  // đọc trạng thái nút và lưu sự kiện nhấn một lần.
  const bool buttonPressed = button.pressed();
  if (buttonPressed)
  {
    // Mỗi sự kiện nhấn đã debounce chỉ đảo LED một lần.
    led.toggle();
    serial_view::printButtonPressed(cfg::kButtonPin);
  }

  // Check if it's time to start a new scan.
  if (!cfg::kEnableWifiScan)
    return;

  // Determine if the scan interval has elapsed.
  const bool intervalElapsed = (millis() - lastScanMs >= cfg::kScanIntervalMs);
  if (buttonPressed || intervalElapsed)
  {
    lastScanMs = millis();
    serial_view::printScanStart();
    wifi_scanner.start();
  }

  // Poll the scanner for its current state.
  switch (wifi_scanner.poll())
  {
  case ScanState::Done:
    serial_view::printNetworks(wifi_scanner);
    wifi_scanner.release();
    break;
  case ScanState::Failed:
    serial_view::printScanFailed();
    break;
  default:
    break;
  }
}
