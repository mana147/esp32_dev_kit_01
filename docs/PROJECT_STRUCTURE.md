# Cấu trúc dự án ESP32 (PlatformIO + Arduino)

Tài liệu mô tả cách tổ chức code chuẩn cho dự án ESP32 và cách dự án `esp32_wifi_scan_01v` áp dụng nó.

## 1. Cây thư mục

```
project/
├── platformio.ini
├── platformio_override.ini   # Cấu hình riêng từng máy (cổng serial), không commit
├── .clang-format             # Quy ước định dạng code
├── README.md / AGENT.md
├── include/                  # Header dùng chung toàn dự án
│   ├── config.h              # Pin, hằng số, interval
│   └── secrets.h.example     # Mẫu SSID/password (secrets.h nằm trong .gitignore)
├── src/
│   └── main.cpp              # Chỉ setup()/loop(), ghép các module
├── lib/                      # Mỗi module là 1 thư mục riêng
│   ├── WifiUtils/src/WifiUtils.{h,cpp}   # Hàm thuần, test được trên PC
│   ├── OuiDb/src/OuiDb.{h,cpp}
│   ├── WifiScanner/src/WifiScanner.{h,cpp}
│   ├── SerialView/src/SerialView.{h,cpp}
│   └── Button/src/Button.{h,cpp}
├── test/
│   ├── test_wifi_utils/test_main.cpp     # native: chạy trên máy tính
│   └── test_hw_blink/test_main.cpp       # on-device: cần board
├── data/                     # File cho LittleFS (web, json) - khi cần
└── docs/                     # Sơ đồ nối dây, ghi chú
```

**Quy ước:**

- `src/` chứa code riêng của ứng dụng, `lib/` chứa module tái sử dụng được. Mỗi thư mục trong `lib/` là một module độc lập.
- `include/` chứa header cấu hình dùng chung.
- Mỗi test là một thư mục `test/test_xxx/` có `test_main.cpp` riêng.

## 2. Phân tầng

```
main.cpp / app        ← điều phối (không chứa logic nghiệp vụ)
   │
   ├── Service        ← WifiScanner, MqttClient, SensorReader
   ├── Driver/HAL     ← Button, Led, Display (bọc phần cứng)
   └── Utils (thuần)  ← WifiUtils, OuiDb (test được trên PC)
```

- Tầng dưới không được `#include` tầng trên.
- Hàm thuần không đụng `Serial` hay `digitalRead`, nhờ vậy test được trên máy tính.
- Dữ liệu đi qua struct (`NetworkInfo`), việc in ra do tầng View (`SerialView`) đảm nhiệm.

## 3. Cấu hình tập trung

```cpp
// include/config.h
#pragma once
#include <stdint.h>

namespace cfg {
constexpr uint32_t kSerialBaud     = 115200;
constexpr uint8_t  kButtonPin      = 15;
constexpr uint32_t kScanIntervalMs = 5000;
constexpr uint32_t kDebounceMs     = 30;
}
```

Thông tin nhạy cảm (SSID, mật khẩu, API key) để trong `secrets.h` và thêm vào `.gitignore`. Repo chỉ chứa `secrets.h.example`.

## 4. Mẫu module (class gọn, non-blocking)

```cpp
class WifiScanner {
 public:
  void begin();
  void start();            // bắt đầu quét async
  ScanState poll();        // Running / Done / Failed
  bool get(int i, NetworkInfo& out) const;
  void release();          // scanDelete()
};
```

## 5. `main.cpp` mỏng

`main.cpp` chỉ khởi tạo module và gọi chúng trong `loop()`; không chứa logic quét, định dạng hay tra cứu.

## 6. `platformio.ini` chuẩn

```ini
[platformio]
default_envs = esp32dev

[env]                              ; dùng chung cho mọi env
platform = espressif32@^6.0.0      ; ghim phiên bản
framework = arduino
monitor_speed = 115200
monitor_filters = esp32_exception_decoder

[env:esp32dev]
board = esp32dev

[env:esp32dev_debug]
extends = env:esp32dev

[env:native]                       ; test thuần trên PC
platform = native
test_framework = unity
```

- Không hard-code `upload_port`. Đặt trong `platformio_override.ini` (đã `.gitignore`) và nạp bằng `extra_configs`.
- Tách env `debug` / `release`.

## 7. Quy ước code

| Hạng mục | Quy ước |
|---|---|
| Tên file | `PascalCase` cho module/class (`WifiScanner.h`) |
| Header | `#pragma once` |
| Hằng số | `constexpr` trong `namespace`, không dùng `#define` |
| Chuỗi cố định | `const char*` hoặc `F("...")`, hạn chế `String` |
| Thời gian | `millis()` thay vì `delay()` |
| Global | Hạn chế; đặt trong `.cpp` dạng `static` hoặc `namespace` |
| Hàm | Mỗi hàm làm một việc, ngắn (khoảng dưới 40 dòng) |
| Giá trị trả về | Luôn kiểm tra mã lỗi (ví dụ `scanNetworks() < 0`) |
| Format | `.clang-format` |

## 8. Khi dự án lớn hơn

- Dùng FreeRTOS task (`xTaskCreatePinnedToCore`) kèm queue giữa các module.
- Dùng máy trạng thái (`enum class State { Idle, Scanning, Showing }`) thay cho nhiều cờ bool.
- Dùng `Preferences` (NVS) để lưu cấu hình và `LittleFS` cho file.
- Dùng CI (GitHub Actions + `pio run` + `pio test -e native`).

## 9. Áp dụng vào dự án hiện tại

| Trước | Sau |
|---|---|
| `wifi_helpers.cpp` (logic + in) | `WifiUtils` (thuần) + `OuiDb` + `SerialView` |
| `main.cpp` chứa logic quét | `WifiScanner` (async) + `main.cpp` mỏng |
| `BUTTON_PIN` trong `main.cpp` | `include/config.h` + class `Button` (debounce) |
| `delay(5000)` chặn chương trình | `millis()` + quét async |
| `scanNetworks() == 0` bỏ sót lỗi | `ScanState::Failed` xử lý riêng |
| Cột "Width" suy đoán từ band | Bỏ (API scan không cung cấp độ rộng kênh) |
| `test_blink.cpp` | `test/test_hw_blink/` + `test/test_wifi_utils/` (native) |
| `upload_port` hard-code | `platformio_override.ini` |

## 10. Lệnh thường dùng

```bash
pio run                       # Build
pio run -t upload             # Nạp
pio device monitor            # Serial monitor
pio test -e native            # Unit test chạy trên PC
pio test -e esp32dev -f test_hw_blink   # Test trên board
```
