#pragma once
#include <WifiScanner.h>

namespace serial_view {

void printButtonPressed(uint8_t pin);
void printScanStart();
void printScanFailed();
void printNetworks(const WifiScanner &scanner);

} // namespace serial_view
