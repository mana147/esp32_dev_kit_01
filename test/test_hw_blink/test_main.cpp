#include <Arduino.h>
#include <unity.h>

const uint8_t LED_PIN = 2;  // LED tích hợp trên hầu hết board ESP32 Dev Module

void setLedState(bool state) {
  digitalWrite(LED_PIN, state ? HIGH : LOW);
}

void test_led_blink_sequence() {
  delay(1000);  // chờ 1 giây trước khi bắt đầu nhấp nháy

  for (int i = 0; i < 3; i++) {
    setLedState(true);
    delay(1000);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_PIN));

    setLedState(false);
    delay(1000);
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_PIN));
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(1000);

  Serial.println("[Blink Test] Starting LED blink test on ESP32 Dev Module");

  UNITY_BEGIN();
  RUN_TEST(test_led_blink_sequence);
  UNITY_END();
}

void loop() {
  // Sau khi test kết thúc, dừng ở trạng thái LOW để dễ quan sát.
  setLedState(false);
  delay(1000);
}
