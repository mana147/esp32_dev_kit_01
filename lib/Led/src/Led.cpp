#include "Led.h"

void Led::begin()
{
  // Khởi tạo LED ở trạng thái tắt, đồng thời đặt _on = false.
  // Đặt mức LOW trước khi chuyển GPIO sang OUTPUT để tránh bật LED ngoài ý muốn.
  set(false);

  // _pin là chân GPIO được truyền vào khi tạo đối tượng Led.
  pinMode(_pin, OUTPUT);
}

void Led::set(bool on)
{
  // LED active HIGH: mức HIGH bật LED, mức LOW tắt LED.
  // Tham số on = true yêu cầu bật; on = false yêu cầu tắt.
  if (on)
  {
    digitalWrite(_pin, HIGH);
  }
  else
  {
    digitalWrite(_pin, LOW);
  }

  // Lưu trạng thái vừa yêu cầu để toggle() và isOn() sử dụng.
  // Đây là trạng thái phần mềm, không phải phép đo LED thực tế có sáng hay không.
  _on = on;
}

void Led::toggle()
{
  // Dựa vào trạng thái đang lưu: đang bật thì tắt, đang tắt thì bật.
  // Gọi set() để cập nhật cả mức GPIO lẫn biến _on tại cùng một nơi.
  if (_on)
  {
    set(false);
  }
  else
  {
    set(true);
  }
}
