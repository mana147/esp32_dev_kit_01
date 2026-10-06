# ESP32 WiFi Scan (esp32_wifi_scan_01v)

Dự án PlatformIO cho board **ESP32 Dev Module**, dùng framework **Arduino**. Mục tiêu của dự án là quét (scan) các mạng WiFi xung quanh bằng ESP32.

> **Trạng thái hiện tại:** dự án mới được khởi tạo từ template PlatformIO. [src/main.cpp](src/main.cpp) vẫn chỉ chứa code mẫu (hàm `myFunction`), chưa có chức năng quét WiFi.

## Phần cứng & cấu hình

Board: ESP32 Dev Module (ESP32-D0WDQ6, cầu nối USB CP2102).

### Thông tin mạch (đọc bằng esptool)

| Mục | Giá trị |
|---|---|
| Chip | ESP32-D0WDQ6, revision v1.0 |
| Tính năng | WiFi, Bluetooth, 2 nhân, 240 MHz |
| Thạch anh | 40 MHz |
| MAC | `80:7d:3a:f3:39:04` |
| Flash | 4 MB (manufacturer `0x20`, device `0x4016`) |
| PSRAM | Không có |
| Cầu nối USB | CP2102 (VID:PID `10C4:EA60`) |
| Cổng serial (macOS) | `/dev/cu.SLAB_USBtoUART` |

### Cấu hình PlatformIO

Tương đương các tuỳ chọn trong menu Tools của Arduino IDE (xem [platformio.ini](platformio.ini)):

| Arduino IDE | platformio.ini |
|---|---|
| Bo mạch: ESP32 Dev Module | `board = esp32dev` |
| Upload Speed: 115200 | `upload_speed = 115200` |
| CPU Frequency: 240MHz | `board_build.f_cpu = 240000000L` |
| Flash Frequency: 80MHz | `board_build.f_flash = 80000000L` |
| Flash Mode: QIO | `board_build.flash_mode = qio` |
| Flash Size: 4MB | `board_upload.flash_size = 4MB` |
| Partition Scheme: Default 4MB with spiffs | `board_build.partitions = default.csv` |
| Core Debug Level: None | `-DCORE_DEBUG_LEVEL=0` |
| Arduino / Events Run On: Core 1 | `-DARDUINO_RUNNING_CORE=1`, `-DARDUINO_EVENT_RUNNING_CORE=1` |
| PSRAM: Disabled | không thêm `-DBOARD_HAS_PSRAM` |
| Cổng kết nối | `upload_port`, `monitor_port` |

Để đọc lại thông tin mạch:

```bash
pio device list
python ~/.platformio/packages/tool-esptoolpy/esptool.py --port /dev/cu.SLAB_USBtoUART flash_id
```

## Cấu trúc thư mục

```
.
├── platformio.ini   # Cấu hình PlatformIO (env: esp32dev)
├── src/
│   └── main.cpp     # Code chính (setup / loop)
├── include/         # Header dùng chung
├── lib/             # Thư viện riêng của dự án
├── test/            # Unit test (PlatformIO Test)
└── .vscode/         # Cấu hình VS Code (extensions, ...)
```

## Yêu cầu

- [VS Code](https://code.visualstudio.com/) + extension [PlatformIO IDE](https://platformio.org/install/ide?install=vscode), hoặc [PlatformIO Core (CLI)](https://docs.platformio.org/en/latest/core/installation/index.html)
- Board ESP32 Dev Module và cáp USB (có dữ liệu)

## Build, nạp và theo dõi

```bash
pio run                      # Build
pio run -t upload            # Build và nạp lên board
pio device monitor           # Mở Serial Monitor (115200 baud)
```

## Hướng phát triển: quét WiFi

Ví dụ tham khảo để thay thế nội dung `main.cpp`:

```cpp
#include <Arduino.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  Serial.println("Dang quet WiFi...");
  int n = WiFi.scanNetworks();

  if (n <= 0) {
    Serial.println("Khong tim thay mang nao");
  } else {
    for (int i = 0; i < n; i++) {
      Serial.printf("%2d | %-32s | RSSI %4d dBm | CH %2d | %s\n",
                    i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.channel(i),
                    WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Secured");
    }
  }
  WiFi.scanDelete();
  delay(5000);
}
```

## Ghi chú

- Thư mục `.pio` (kết quả build) và một số file cục bộ của `.vscode` đã được loại trừ trong [.gitignore](.gitignore).
- ESP32 chỉ quét được mạng WiFi 2.4 GHz.
