#include "config.h"
#include <Arduino.h>
#include <Button.h>
#include <SerialView.h>
#include <WifiScanner.h>

namespace
{
WifiScanner scanner;
Button button(cfg::kButtonPin, cfg::kDebounceMs);
uint32_t lastScanMs = 0;
} // namespace

void setup()
{
  Serial.begin(cfg::kSerialBaud); // note : 115200
  delay(cfg::kSerialBootDelayMs); // Wait for the serial port to initialize.

  // Initialize the button.
  button.begin();

  // Initialize the WiFi scanner if enabled.
  if (cfg::kEnableWifiScan)
    scanner.begin();

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
    scanner.start();
  }

  // Poll the scanner for its current state.
  switch (scanner.poll())
  {
  case ScanState::Done:
    serial_view::printNetworks(scanner);
    scanner.release();
    break;
  case ScanState::Failed:
    serial_view::printScanFailed();
    break;
  default:
    break;
  }
}
