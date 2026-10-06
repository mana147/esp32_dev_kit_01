#pragma once
#include <stdint.h>

namespace oui_db {

// Looks up the vendor from the first 3 bytes of a BSSID.
// Returns "Private" for locally-administered (randomized) addresses
// and "Unknown" when the OUI is not in the table or bssid is null.
const char *vendorFromBssid(const uint8_t *bssid);

} // namespace oui_db
