#include "OuiDb.h"

namespace oui_db {

namespace {

struct OuiEntry
{
  uint8_t oui[3];
  const char *vendor;
};

constexpr OuiEntry kTable[] = {
    {{0x00, 0x0C, 0x43}, "Cisco"},
    {{0x00, 0x17, 0x9A}, "Cisco"},
    {{0x00, 0x1A, 0x2B}, "Intel"},
    {{0x00, 0x50, 0x56}, "VMware"},
    {{0x00, 0x80, 0x9F}, "Netgear"},
    {{0x00, 0x90, 0x4C}, "TP-Link"},
    {{0x00, 0x1D, 0xAA}, "TP-Link"},
    {{0x70, 0x4F, 0x57}, "TP-Link"},
    {{0x88, 0x1F, 0xA1}, "Google"},
    {{0xA4, 0x2B, 0xB0}, "Apple"},
    {{0xA8, 0x40, 0x41}, "Apple"},
};

constexpr uint8_t kLocallyAdministeredBit = 0x02;

} // namespace

const char *vendorFromBssid(const uint8_t *bssid)
{
  if (bssid == nullptr)
    return "Unknown";

  if (bssid[0] & kLocallyAdministeredBit)
    return "Private";

  for (const OuiEntry &e : kTable)
  {
    if (e.oui[0] == bssid[0] && e.oui[1] == bssid[1] && e.oui[2] == bssid[2])
      return e.vendor;
  }
  return "Unknown";
}

} // namespace oui_db
