# Hướng dẫn AI agent — esp32_wifi_scan_01v

Tài liệu này hướng dẫn các AI agent đọc, sửa và kiểm chứng dự án PlatformIO/Arduino cho ESP32. Phạm vi áp dụng: toàn bộ repository. `AGENTS.md` ở thư mục gốc dẫn tới tài liệu này; duy trì nội dung hướng dẫn tại đây để tránh hai bản khác nhau.

## 1. Cách làm việc với người dùng

- Trao đổi và viết tài liệu bằng tiếng Việt. Người dùng có kinh nghiệm web Mid-level (2–5 năm), Embedded khoảng 4 năm và DevOps Junior; giải thích quyết định kỹ thuật, tác động và cách kiểm tra, không cần giảng lại kiến thức lập trình căn bản.
- Đọc yêu cầu mới nhất, kiểm tra `git status --short --branch` và đọc file liên quan trước khi sửa. Giữ nguyên thay đổi có sẵn của người dùng; không reset, checkout đè hoặc dọn file ngoài phạm vi công việc.
- Khi yêu cầu là đọc, đánh giá hoặc lập kế hoạch, chỉ thực hiện phạm vi đó. Khi được yêu cầu triển khai, hoàn thành thay đổi và kiểm chứng phù hợp.
- Phân biệt rõ: cấu hình trong file, hành vi đã triển khai, ví dụ/đề xuất trong tài liệu và kết quả đã kiểm thử trên phần cứng. Thiếu thông tin thì ghi “Chưa cung cấp” hoặc “Chưa kiểm chứng”.
- Cuối công việc, báo file đã sửa, hành vi thay đổi, lệnh kiểm tra và kết quả; nêu phần chưa kiểm chứng nếu có.

## 2. Đọc dự án theo thứ tự

1. `platformio.ini`, `.gitignore`, `.clang-format`; đọc `platformio_override.ini` nếu có để biết cấu hình cục bộ.
2. `include/config.h`, `src/main.cpp` để xác định tính năng đang bật và luồng chạy thực tế.
3. Header và implementation của module liên quan trong `lib/*/src/`.
4. `test/test_wifi_utils/test_main.cpp`, `test/test_hw_blink/test_main.cpp`.
5. [Cấu trúc dự án](docs/PROJECT_STRUCTURE.md), [tham khảo ESP32](docs/ESP32_DOIT_DEVKIT_V1_REFERENCE.md), [tham khảo OLED](docs/OLED_0_96_6PIN_REFERENCE.md) khi cần kiến trúc hoặc đấu nối.

Mã nguồn và cấu hình hiện tại xác định hành vi triển khai. `docs/` có cả mẫu tổ chức, ví dụ độc lập và phương án mở rộng; đối chiếu trước khi áp dụng. `include/README`, `lib/README`, `test/README` là hướng dẫn mẫu của PlatformIO. Dự án hiện chưa có `README.md` ở gốc.

## 3. Mục tiêu và trạng thái hiện tại

- Firmware C++ dùng Arduino trên ESP32 cổ điển, mục tiêu quét mạng WiFi và xuất bảng kết quả qua Serial. Chưa có web server, kết nối vào access point, MQTT hoặc OLED trong firmware.
- **`cfg::kEnableWifiScan = false` trong `include/config.h`: quét WiFi hiện bị tắt.** `setup()` vẫn khởi tạo LED, mở Serial và khởi tạo nút; `loop()` đọc nút, đảo trạng thái LED và in sự kiện nhấn qua Serial rồi return. Không có log scan hoặc kết quả WiFi trong cấu hình này. Không tự đổi cờ khi làm công việc khác.
- Khi cờ được bật: quét ngay ở lần chạy `loop()` đầu, quét lại theo chu kỳ 5000 ms hoặc khi nút phát sự kiện nhấn; quét chạy bất đồng bộ.
- Nút hiện dùng **GPIO15, active HIGH, `INPUT_PULLDOWN`**, nối nút giữa GPIO15 và 3V3, debounce 30 ms. Mỗi lần nhấn in một dòng `[Button] GPIO15 pressed (HIGH)` qua Serial 115200 baud; giữ nút không in lặp. Đây là nút ngoài, khác nút BOOT tại GPIO0.
- LED onboard dùng **`cfg::kLedPin = 2`, active HIGH**: tắt khi khởi tạo, mỗi sự kiện nhấn nút đảo trạng thái LED một lần; giữ nút không đảo lặp. Hành vi này vẫn chạy khi quét WiFi bị tắt. Chân/cực tính LED trên board thực tế chưa kiểm chứng trong lần sửa này.
- Bảng Serial gồm SSID, BSSID, channel, band, security, RSSI, Signal% và vendor. RSSI là dBm; Signal% là phép quy đổi tuyến tính, không phải phép đo chất lượng liên kết thực tế.
- OLED chỉ có tài liệu và ảnh tham khảo. Không có module Display hoặc dependency U8g2 trong `platformio.ini`.

Các giá trị trên là trạng thái tại lúc viết hướng dẫn; agent phải đọc lại file trước khi kết luận hoặc thay đổi.

## 4. Bản đồ mã nguồn và trách nhiệm

| Đường dẫn | Trách nhiệm | Khi cần sửa |
| --- | --- | --- |
| `src/main.cpp` | `setup()`/`loop()`, ghép scanner, nút, LED onboard và Serial view | Điều phối, lịch quét, bật/tắt luồng ứng dụng |
| `include/config.h` | `namespace cfg`: baud, boot delay, GPIO, debounce, cờ bật scan, interval | Cấu hình ứng dụng và pin |
| `lib/Button/src/Button.{h,cpp}` | Đọc nút, debounce và sự kiện nhấn một lần | Input phần cứng |
| `lib/Led/src/Led.{h,cpp}` | Khởi tạo LED active HIGH, lưu trạng thái, bật/tắt và đảo trạng thái | Output phần cứng |
| `lib/WifiScanner/src/WifiScanner.{h,cpp}` | Bọc WiFi Arduino, scan async, trạng thái và dữ liệu `NetworkInfo` | Logic quét, vòng đời kết quả, tên security |
| `lib/SerialView/src/SerialView.{h,cpp}` | In trạng thái/bảng ra Serial | Định dạng đầu ra |
| `lib/WifiUtils/src/WifiUtils.{h,cpp}` | Hàm thuần: band theo channel, phần trăm theo RSSI | Logic tính toán chạy được trên máy tính |
| `lib/OuiDb/src/OuiDb.{h,cpp}` | Tra vendor từ 3 byte đầu BSSID | Bảng OUI và cách xử lý địa chỉ |
| `test/test_wifi_utils/test_main.cpp` | Unity trên máy tính: band, Signal%, vendor | Kiểm chứng hàm thuần |
| `test/test_hw_blink/test_main.cpp` | Firmware test LED GPIO2 | Kiểm tra GPIO trên board |
| `docs/`, `img/` | Kiến trúc, phần cứng, ảnh tham khảo | Tài liệu và mapping đề xuất |

Giữ `main.cpp` làm tầng điều phối. `SerialView` phụ thuộc `WifiScanner`, `WifiUtils`, `OuiDb`; `WifiScanner`, `Button` và `Led` phụ thuộc Arduino/phần cứng. `WifiUtils` và `OuiDb` phải giữ độc lập với Arduino, `WiFi`, `Serial`, GPIO để chạy test native. Tầng dưới không include tầng điều phối hoặc view.

## 5. Luồng chạy và các ràng buộc cần giữ

### Quét WiFi

1. `setup()` gọi `led.begin()` để tắt LED GPIO2 và cấu hình OUTPUT, mở Serial 115200 baud, chờ 1000 ms, gọi `button.begin()`; chỉ gọi `scanner.begin()` khi cờ scan bật.
2. `scanner.begin()` cấu hình `WIFI_STA`, gọi `WiFi.disconnect(true)` và chờ 100 ms. Không có lệnh kết nối SSID/password.
3. `lastScanMs = millis() - cfg::kScanIntervalMs` cho phép quét ngay khi bắt đầu `loop()`.
4. Mỗi vòng lặp cập nhật nút và đọc `pressed()` một lần vào `buttonPressed`; đảo trạng thái LED và in sự kiện nhấn kể cả khi scan bị tắt. Khi scan được bật và tới lịch hoặc có sự kiện nhấn đã lưu, ứng dụng cập nhật mốc thời gian, in trạng thái và gọi `scanner.start()`.
5. `start()` bỏ qua yêu cầu nếu `_running` đang true; nếu không, giải phóng kết quả cũ và gọi `WiFi.scanNetworks(true, false)` (async, không yêu cầu hiển thị mạng ẩn).
6. `poll()` trả `Idle`, `Running`, `Done` hoặc `Failed`. Khi `Done`, ứng dụng in kết quả rồi gọi `release()`; khi `Failed`, in lỗi. Số mạng bằng 0 là quét thành công không tìm thấy mạng, khác lỗi âm.

Ràng buộc khi sửa:

- Đọc/sao chép dữ liệu cần dùng trước `release()`/`WiFi.scanDelete()`. Nếu thêm màn hình giữ kết quả qua nhiều vòng lặp, lưu snapshot riêng; không tiếp tục truy cập dữ liệu scan đã giải phóng.
- `Done` chỉ được trả ở lần `poll()` nhận kết quả hoàn tất; lần sau trả `Idle`. Consumer phải xử lý ngay hoặc lưu trạng thái riêng.
- `release()` hiện giải phóng kết quả và đặt `_count = 0`; không phải API hủy scan, không đặt `_running = false`.
- Giữ phép tính thời gian bằng hiệu unsigned (`now - previousMs >= interval`) để chịu được vòng tràn `millis()`.
- Khi scan đang chạy, `main.cpp` vẫn có thể in “Scanning WiFi networks...” và cập nhật lịch dù `start()` bỏ qua yêu cầu. Đừng mô tả nút là có hàng đợi hoặc có khả năng restart scan.
- `start()` hiện chưa kiểm tra giá trị trả về tức thời của `scanNetworks()`; trạng thái lỗi được xử lý qua `poll()`. Nếu sửa xử lý lỗi, đối chiếu API của framework thực tế và cả giá trị biểu thị scan đang chạy.

### Nút, LED và các hàm thuần

- `Button::update()` cần được gọi thường xuyên; tránh thêm tác vụ chặn dài vào `loop()`. `pressed()` đọc rồi xóa sự kiện; `isDown()` trả trạng thái ổn định.
- `Led::begin()` khởi tạo về trạng thái tắt; `set(bool)` bật/tắt, `toggle()` đảo trạng thái, `isOn()` trả trạng thái đã lưu trong module. `main.cpp` gọi `led.toggle()` khi có sự kiện nhấn, không quản lý trạng thái LED hoặc ghi GPIO trực tiếp.
- Log nút chạy trước nhánh return khi scan bị tắt. Dùng lại `buttonPressed` khi quyết định quét; không gọi `pressed()` lần thứ hai vì lần đầu đã xóa sự kiện.
- `bandFromChannel()` trả `2.4GHz` cho 1–14, `5GHz` cho 36–165, còn lại `Unknown`. Đây là helper phân loại; không suy ra ESP32 của dự án có thể scan 5 GHz.
- `signalPercentFromRssi()` ánh xạ [-90, -30] dBm sang [0, 100], chặn ngoài khoảng và tính bằng số nguyên.
- `vendorFromBssid()` trả `Unknown` khi null/không có OUI, `Private` khi có bit locally administered (`0x02`), còn lại tra bảng nhỏ trong `OuiDb.cpp`. Bảng chưa phải cơ sở dữ liệu OUI đầy đủ/đã xác minh mọi entry; cần nguồn đáng tin cậy khi cập nhật vendor.

## 6. Môi trường PlatformIO

| Mục | Cấu hình trong repository |
| --- | --- |
| Môi trường mặc định | `esp32dev` |
| Platform ESP32 | `espressif32@^6.3.1` — ràng buộc phiên bản, không khóa đúng một bản |
| Board / framework | `esp32dev` / `arduino` |
| CPU / flash clock / mode | 240 MHz / 80 MHz / QIO |
| Flash / partition | 4 MB / `default.csv` |
| Upload / Serial Monitor | **921600** / **115200** baud; hai tốc độ khác nhau |
| Flags firmware | `-Wall`, `-Wextra`, `CORE_DEBUG_LEVEL=0`, Arduino và event chạy core 1 |
| Debug env | `esp32dev_debug` kế thừa firmware env, đặt `CORE_DEBUG_LEVEL=4`; không tự cấu hình probe JTAG |
| Native env | Platform `native`, Unity, `-std=c++17` |
| Phân loại test | ESP32 bỏ `test_wifi_utils`; native bỏ `test_hw_*` |

Không thêm `BOARD_HAS_PSRAM` khi chưa xác nhận board. `native` dùng C++17 không có nghĩa firmware cũng dùng chuẩn đó; kiểm tra compiler flags của build ESP32 trước khi dùng tính năng ngôn ngữ mới.

`platformio.ini` nạp `platformio_override.ini` qua `extra_configs`. File override được ignore và hiện đặt cổng cho `env:esp32dev`; cấu hình riêng này không bảo đảm cổng đó tồn tại hoặc áp dụng giống nhau cho env debug. Dùng `pio device list`, kiểm tra cấu hình hiệu lực rồi đặt cổng đúng máy. Không hard-code cổng của máy hiện tại vào cấu hình chia sẻ.

Nếu `pio` chưa nằm trong PATH nhưng Core đã được PlatformIO IDE cài, thử `~/.platformio/penv/bin/pio` thay cho `pio` trong các lệnh dưới.

## 7. Lệnh và cách kiểm chứng

Chạy tại thư mục gốc dự án:

```bash
pio run -e esp32dev                       # Build firmware, không nạp
pio run -e esp32dev_debug                 # Build khi thay cấu hình debug
pio test -e native -f test_wifi_utils      # Test hàm thuần, không cần board
pio device list                          # Xác định cổng thiết bị
git diff --check -- AGENT.md AGENTS.md     # Khi chỉ sửa hướng dẫn agent
```

Khi sửa mã nguồn, chạy `git diff --check` trên các file đã sửa; phân biệt lỗi có sẵn với lỗi mới. Khi đổi logic hàm thuần, chạy native test và bổ sung case có ý nghĩa cho hành vi mới/biên/lỗi. Khi đổi firmware, chạy build `esp32dev`; nếu đổi debug env, kiểm tra cả env debug. Sửa tài liệu chỉ cần đối chiếu nội dung, link và diff; không bắt buộc build lại nếu không có điều gì cần xác minh.

Các lệnh sau tác động phần cứng, chỉ chạy khi công việc có yêu cầu hoặc đã được cho phép nạp/test board:

```bash
pio run -e esp32dev -t upload
pio device monitor -e esp32dev
pio test -e esp32dev -f test_hw_blink
```

Hardware test nạp **firmware test** lên board, thay chương trình đang chạy; nạp lại firmware ứng dụng nếu công việc cần trả board về ứng dụng. Test blink chỉ kiểm tra mức GPIO2 qua `digitalRead()`, không chứng minh LED thực sự sáng hoặc WiFi/OLED hoạt động.

Khi kiểm chứng WiFi trên board, xác nhận cờ scan đã được bật theo yêu cầu, đúng cổng/baud, rồi quan sát quét lúc boot, chu kỳ, nút nhấn, bảng kết quả và các trạng thái lỗi/không có mạng nếu tái hiện được. Build thành công không thay thế kiểm tra trên board. Với cờ false, việc không có log scan là đúng luồng hiện tại.

Ghi nhận ngày **06/10/2026** khi viết lại tài liệu: `pio run -e esp32dev` thành công; `pio test -e native` có **3/3 test đạt**. Build dùng Espressif32 **6.3.1**, Arduino-ESP32 **2.0.9**. Đây là snapshot của môi trường cục bộ, không bảo đảm phiên bản ở máy khác; chưa nạp/test board trong lần cập nhật này. Cờ scan false có thể khiến compiler loại bỏ phần scan không được dùng; khi bật scan hoặc sửa scanner phải build lại cấu hình tương ứng.

## 8. Quy ước khi phát triển

- Module tái sử dụng: `lib/<Module>/src/<Module>.h` và `.cpp`; header dùng `#pragma once`. Test: `test/test_<feature>/test_main.cpp`.
- Pin và cấu hình ứng dụng tập trung tại `include/config.h`, dùng `constexpr` trong namespace; hằng nội bộ module đặt trong `.cpp`/anonymous namespace.
- Theo `.clang-format`: LLVM, indent 2 spaces, Allman braces, column limit 120. Format phần đã sửa, tránh format hàng loạt thay đổi không liên quan.
- Giữ logic tính toán riêng khỏi phần cứng và định dạng đầu ra. Dữ liệu mạng đi qua `NetworkInfo`; không thêm in Serial vào scanner hoặc utils.
- Ưu tiên `millis()`/state machine cho tác vụ lặp. Boot delay và delay của hardware test là các phần đang tồn tại; tránh lan thêm `delay()` vào luồng ứng dụng.
- Hạn chế cấp phát động/ghép `String` trong luồng chạy liên tục; `NetworkInfo::ssid` hiện vẫn dùng Arduino `String`, không tự thay API khi nhiệm vụ không cần.
- Kiểm tra lỗi và vòng đời tài nguyên khi dùng API phần cứng. Không tự thêm FreeRTOS task, MQTT, filesystem hoặc CI chỉ vì tài liệu có gợi ý mở rộng.
- Comment mới nên dùng tiếng Việt, tập trung giải thích lý do/ràng buộc; giữ identifier và tên API rõ ràng theo module hiện tại.
- Không sửa `.pio/` hoặc file IDE sinh tự động (`.vscode/c_cpp_properties.json`, `.vscode/launch.json`). Cấu hình build lấy từ `platformio.ini`, không từ danh sách include của IDE.
- `.gitignore` đã loại `platformio_override.ini`, `include/secrets.h`, `.pio/` và file IDE cục bộ. Không đưa credential vào code/log/docs; chỉ thêm mẫu secrets nếu tính năng thực sự cần. Hiện chưa có `secrets.h.example`, `data/` hoặc workflow CI, dù tài liệu kiến trúc có minh họa.

## 9. Phần cứng và tài liệu tham khảo

Ghi nhận phần cứng từ **AGENT.md cũ**, giữ để tham chiếu, **chưa đọc lại thiết bị trong lần cập nhật này**:

| Mục | Ghi nhận trước đây |
| --- | --- |
| Chip | ESP32-D0WDQ6, revision v1.0 |
| Khả năng / thạch anh | WiFi, Bluetooth, 2 core, 240 MHz / 40 MHz |
| Flash | 4 MB, manufacturer `0x20`, device `0x4016` |
| PSRAM | Không có theo ghi nhận cũ |
| USB–UART | CP2102, VID:PID `10C4:EA60` |
| MAC | `80:7d:3a:f3:39:04` — chỉ của thiết bị đã ghi nhận |
| Cổng cũ trên macOS | `/dev/cu.SLAB_USBtoUART` — không dùng làm mặc định cho mọi máy |

Khi đổi pin/đấu nối, đọc [tài liệu ESP32](docs/ESP32_DOIT_DEVKIT_V1_REFERENCE.md) và đối chiếu đúng board. Tài liệu áp dụng cho ảnh DOIT DevKit V1 30 chân, không mặc nhiên áp dụng ESP32-C3/S2/S3. GPIO15 của nút hiện tại là strapping pin; cân nhắc mức tại reset nếu đổi wiring. Đề xuất GPIO27 active LOW trong tài liệu **chưa được triển khai** và khác nút hiện tại.

Khi được yêu cầu tích hợp OLED, đọc [tài liệu OLED](docs/OLED_0_96_6PIN_REFERENCE.md): mã module/driver/bus thực tế chưa xác nhận. SSD1306 128×64, U8g2, SPI SCK18/MOSI23/DC16/RES17 và CS không đưa ra header là **phương án tham khảo có điều kiện**, không phải phần cứng đã kiểm chứng. Không kết luận I²C chỉ từ nhãn SCL/SDA hoặc suy ra CS nối LOW chỉ từ việc thiếu chân CS. Ví dụ OLED đã có ghi nhận build riêng trong tài liệu, chưa có ghi nhận chạy trên màn thực tế.

Ảnh trong `img/` là ảnh tham khảo; giữ nguồn/chú thích/giấy phép trong tài liệu khi cập nhật. Không dùng ảnh mẫu để khẳng định cấu hình board hoặc màn đang nối với máy.

## 10. Duy trì hướng dẫn

Khi nhiệm vụ làm thay đổi tính năng, cấu hình, module hoặc lệnh kiểm tra, cập nhật phần liên quan của `AGENT.md` và tài liệu thực sự bị ảnh hưởng. Giữ mô tả hiện trạng tách khỏi đề xuất. Không biến các hạn chế được liệt kê ở đây thành yêu cầu tự sửa toàn dự án nếu người dùng chưa yêu cầu.
