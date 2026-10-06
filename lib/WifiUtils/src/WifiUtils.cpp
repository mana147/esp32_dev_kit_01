#include "WifiUtils.h"

namespace wifi_utils {

namespace {
constexpr int kRssiMin = -90;
constexpr int kRssiMax = -30;
} // namespace

const char *bandFromChannel(int channel)
{
  if (channel >= 1 && channel <= 14)
    return "2.4GHz";
  if (channel >= 36 && channel <= 165)
    return "5GHz";
  return "Unknown";
}

int signalPercentFromRssi(int rssi)
{
  if (rssi <= kRssiMin)
    return 0;
  if (rssi >= kRssiMax)
    return 100;
  return (rssi - kRssiMin) * 100 / (kRssiMax - kRssiMin);
}

} // namespace wifi_utils
