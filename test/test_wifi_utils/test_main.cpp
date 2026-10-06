#include <unity.h>
#include <OuiDb.h>
#include <WifiUtils.h>
#include <string.h>

void setUp() {}
void tearDown() {}

void test_band_from_channel()
{
  TEST_ASSERT_EQUAL_STRING("2.4GHz", wifi_utils::bandFromChannel(1));
  TEST_ASSERT_EQUAL_STRING("2.4GHz", wifi_utils::bandFromChannel(14));
  TEST_ASSERT_EQUAL_STRING("5GHz", wifi_utils::bandFromChannel(36));
  TEST_ASSERT_EQUAL_STRING("5GHz", wifi_utils::bandFromChannel(165));
  TEST_ASSERT_EQUAL_STRING("Unknown", wifi_utils::bandFromChannel(0));
  TEST_ASSERT_EQUAL_STRING("Unknown", wifi_utils::bandFromChannel(20));
}

void test_signal_percent()
{
  TEST_ASSERT_EQUAL_INT(0, wifi_utils::signalPercentFromRssi(-100));
  TEST_ASSERT_EQUAL_INT(0, wifi_utils::signalPercentFromRssi(-90));
  TEST_ASSERT_EQUAL_INT(50, wifi_utils::signalPercentFromRssi(-60));
  TEST_ASSERT_EQUAL_INT(100, wifi_utils::signalPercentFromRssi(-30));
  TEST_ASSERT_EQUAL_INT(100, wifi_utils::signalPercentFromRssi(-10));
}

void test_vendor_lookup()
{
  const uint8_t apple[6] = {0xA4, 0x2B, 0xB0, 1, 2, 3};
  const uint8_t unknown[6] = {0x00, 0x11, 0x22, 1, 2, 3};
  const uint8_t randomized[6] = {0x02, 0x11, 0x22, 1, 2, 3};

  TEST_ASSERT_EQUAL_STRING("Apple", oui_db::vendorFromBssid(apple));
  TEST_ASSERT_EQUAL_STRING("Unknown", oui_db::vendorFromBssid(unknown));
  TEST_ASSERT_EQUAL_STRING("Private", oui_db::vendorFromBssid(randomized));
  TEST_ASSERT_EQUAL_STRING("Unknown", oui_db::vendorFromBssid(nullptr));
}

int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_band_from_channel);
  RUN_TEST(test_signal_percent);
  RUN_TEST(test_vendor_lookup);
  return UNITY_END();
}
