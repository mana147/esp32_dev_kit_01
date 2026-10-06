#pragma once
#include <stdint.h>

namespace wifi_utils {

// "2.4GHz", "5GHz" or "Unknown".
const char *bandFromChannel(int channel);

// Maps RSSI (dBm) linearly from [-90, -30] to [0, 100].
int signalPercentFromRssi(int rssi);

} // namespace wifi_utils
